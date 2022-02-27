#include "serial.h"

#ifdef __AVR__

#define USART0_TX_BUFFER_SIZE 128

namespace {
    
static volatile uint8_t USART0_TX_BUFFER[USART0_TX_BUFFER_SIZE];
static volatile uint8_t USART0_TX_BUFFER_HEAD;
static volatile uint8_t USART0_TX_BUFFER_TAIL;

ISR(USART0_TX_vect) {
    if (USART0_TX_BUFFER_HEAD != USART0_TX_BUFFER_TAIL) {
        UDR0 = USART0_TX_BUFFER[USART0_TX_BUFFER_TAIL];
        if (++USART0_TX_BUFFER_TAIL >= USART0_TX_BUFFER_SIZE) USART0_TX_BUFFER_TAIL = 0;
    }
}

}

namespace mcu {

Serial0::Serial0() {
    
    USART0_TX_BUFFER_HEAD = 0;
    USART0_TX_BUFFER_TAIL = 0;
    // Try u2x mode first
    uint16_t baud_setting = (F_CPU / 4 / SERIAL0_BAUD - 1) / 2;
    UCSR0A = 1 << U2X1;
    // hardcoded exception for 57600 for compatibility with the bootloader
    // shipped with the Duemilanove and previous boards and the firmware
    // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
    // be > 4095, so switch back to non-u2x mode if the baud rate is too
    // low.
    //         if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting >4095))
    //         {
    //             UCSR0A = 0;
    //             baud_setting = (F_CPU / 8 / baud - 1) / 2;
    //         }
    
    // assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
    UBRR0H = baud_setting >> 8;
    UBRR0L = baud_setting;
    UCSR0B = ((1<<TXEN0) | (1<<TXCIE0)); // (1<<RXEN0) | (1<<RXCIE0)
    UCSR0C = SERIAL0_CONFIG; //((1<<UCSZ01) | (1<<UCSZ00));
}


void Serial0::write(const uint8_t data) {
    uint8_t i = (USART0_TX_BUFFER_HEAD + 1 >= USART0_TX_BUFFER_SIZE) ? 0 : USART0_TX_BUFFER_HEAD + 1;
    // ждать освобождения места в буфере
    while ( (i + 1) == USART0_TX_BUFFER_TAIL);
    
    // Не сохранять новые данные если нет места
    if (i != USART0_TX_BUFFER_TAIL) {
        USART0_TX_BUFFER[USART0_TX_BUFFER_HEAD] = data;
        USART0_TX_BUFFER_HEAD = i;
    }
    while (!(UCSR0A & (1<<UDRE0)));
    if (USART0_TX_BUFFER_HEAD != USART0_TX_BUFFER_TAIL) {
        UDR0 = USART0_TX_BUFFER[USART0_TX_BUFFER_TAIL];
        if (++USART0_TX_BUFFER_TAIL >= USART0_TX_BUFFER_SIZE) USART0_TX_BUFFER_TAIL = 0;  // хвост двигаем
        //UDR0 = c;
    }
}

}
#else
namespace mcu {
    
    Serial0::Serial0() { }
    
    void Serial0::write(const uint8_t data) {
        std::cout << (char)data;
    }
    
}
#endif



