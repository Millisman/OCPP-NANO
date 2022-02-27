#pragma once

#ifdef __AVR__
#include <avr/interrupt.h>
#endif

namespace {
static volatile uint16_t GSM_RX_BUFF_SIZE;
static volatile uint16_t GSM_TX_BUFF_SIZE;
static volatile uint8_t *_SIM800_TX_BUFF;
static volatile uint8_t *Rx_BUF;
static volatile uint16_t Rx_POS = 0;
static volatile uint16_t _SIM800_TX_BUFF_HEAD = 0;
static volatile uint16_t _SIM800_TX_BUFF_TAIL = 0;
static volatile bool RxLF = false;
static volatile bool RxOK = false;
static volatile bool RxER = false;
static volatile bool RxPP = false;
static volatile bool RxSC = false;
static volatile bool RxOF = false;
}

#ifdef __linux__

#include <stdint.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>

const char* _port = "/dev/ttyUSB0";
static bool _isOpen;
static int _fd;
static speed_t _speed;
static tcflag_t _dataBits;
static bool need_init = true;

void tty_open() {
    need_init = false;
    _dataBits = CS8;
    _speed = B115200;
    
    struct termios config;
    
    _fd = ::open(_port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (_fd == -1) {
        exit(1);
    }
    
    if (!isatty(_fd)) {
        if (_fd >= 0) ::close(_fd);
        exit(1);
    }
    
    if (tcgetattr(_fd, &config) < 0) {
        exit(1);
    }
    
    // Configuration example taken from
    // https://en.wikibooks.org/wiki/Serial_Programming/termios
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    config.c_oflag = 0;
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= _dataBits;
    config.c_cc[VMIN] = 1;
    config.c_cc[VTIME] = 0;
    
    if (cfsetispeed(&config, _speed) < 0 || cfsetospeed(&config, _speed) < 0) {
        exit(1);
    }
    
    if (tcsetattr(_fd, TCSAFLUSH, &config) < 0) {
        exit(1);
    }
    
    _isOpen = true;

}



void service_tty() {
    if (!_isOpen) return;
    uint8_t data;
    while (::read(_fd, &data, 1) == 1) {
        Rx_BUF[Rx_POS] = data;
        RxLF = ((Rx_BUF[Rx_POS-1] == 13)  && (Rx_BUF[Rx_POS] == 10)); //  { RxLF = true; } else // EOL
        RxPP = ((Rx_BUF[Rx_POS-1] == '>') && (Rx_BUF[Rx_POS] == ' ')); // { RxPP = true; Rx_POS = 0; } // PROMPT
        if ((Rx_POS > 2) && RxLF && !RxPP && (Rx_BUF[Rx_POS-3] == 'O') && (Rx_BUF[Rx_POS-2] == 'K')) {
            RxOK = true;
            RxSC = true; // Rx_POS = 0;
        } else if ((Rx_POS > 6) && RxLF && !RxPP &&
            (Rx_BUF[Rx_POS-6] == 'E') && 
            (Rx_BUF[Rx_POS-5] == 'R') &&
            (Rx_BUF[Rx_POS-4] == 'R') &&
            (Rx_BUF[Rx_POS-3] == 'O') &&
            (Rx_BUF[Rx_POS-2] == 'R')
        ) 
        {
            RxER = true;
            RxSC = true;
        } else {
            RxSC = true; // Rx_POS = 0;
        }
        //             RxLF = false;
        Rx_POS++;
        if (Rx_POS >= GSM_RX_BUFF_SIZE) {
            Rx_POS = 0;
            
            RxOK = false;
            RxER = false;
            RxPP = false;
            RxSC = false;
            RxOF = true;
        }
    }
}

void _hal_modem_write(const char c) {
    if (!_isOpen) return;
    ::write(_fd, &c, 1);
}

#endif // __linux__




namespace {

void _hal_modem_init_port() {
#ifdef __linux__
    tty_open();
#endif
    
#ifdef __AVR__
    UCSR1A = 1 << U2X1;
    UBRR1H = ((20000000 / 4 / 57600 - 1) / 2) >> 8;
    UBRR1L = ((20000000 / 4 / 57600 - 1) / 2) & 0xFF;
    UCSR1B = ((1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1) | (1<<TXCIE1));
#endif
}




#ifdef __AVR__
ISR(USART1_TX_vect) {
    if (_SIM800_TX_BUFF_HEAD != _SIM800_TX_BUFF_TAIL) {
        uint8_t c = _SIM800_TX_BUFF[_SIM800_TX_BUFF_TAIL];
        if (++_SIM800_TX_BUFF_TAIL >= GSM_TX_BUFF_SIZE) _SIM800_TX_BUFF_TAIL = 0;  // хвост двигаем
        UDR1 = c;
    }
};

ISR(USART1_RX_vect) {
    if (bit_is_set(UCSR1A, UPE1)) {
        UDR1; // parity error! read
    } else {
        Rx_BUF[Rx_POS] = UDR1;
        RxLF = ((Rx_BUF[Rx_POS-1] == 13)  && (Rx_BUF[Rx_POS] == 10)); //  { RxLF = true; } else // EOL
        RxPP = ((Rx_BUF[Rx_POS-1] == '>') && (Rx_BUF[Rx_POS] == ' ')); // { RxPP = true; Rx_POS = 0; } // PROMPT
        if ((Rx_POS > 2) && RxLF && !RxPP && (Rx_BUF[Rx_POS-3] == 'O') && (Rx_BUF[Rx_POS-2] == 'K')) {
            RxOK = true;
            RxSC = true; // Rx_POS = 0;
        } else if ((Rx_POS > 6) && RxLF && !RxPP &&
            (Rx_BUF[Rx_POS-6] == 'E') && 
            (Rx_BUF[Rx_POS-5] == 'R') &&
            (Rx_BUF[Rx_POS-4] == 'R') &&
            (Rx_BUF[Rx_POS-3] == 'O') &&
            (Rx_BUF[Rx_POS-2] == 'R')
        ) 
        {
            RxER = true;
            RxSC = true;
        } else {
            RxSC = true; // Rx_POS = 0;
        }
        //             RxLF = false;
        Rx_POS++;
        if (Rx_POS >= GSM_RX_BUFF_SIZE) {
            Rx_POS = 0;
            
            RxOK = false;
            RxER = false;
            RxPP = false;
            RxSC = false;
            RxOF = true;
        }
    }
}



void _hal_modem_write(const char c) {
    // ------------- direct write ------------
    // while (!(UCSR1A & (1<<UDRE1)));
    // UDR1 = data;
    // ------------- buffer write ------------
    uint16_t i = (uint16_t)(_SIM800_TX_BUFF_HEAD + 1 >= GSM_TX_BUFF_SIZE) ? 0 : _SIM800_TX_BUFF_HEAD + 1;
    while ( (i + 1) == _SIM800_TX_BUFF_TAIL);
    if (i != _SIM800_TX_BUFF_TAIL) {
        _SIM800_TX_BUFF[_SIM800_TX_BUFF_HEAD] = c;
        _SIM800_TX_BUFF_HEAD = i;
    }
    
    // ---------- start Transmission ---------
    while (!(UCSR1A & (1<<UDRE1)));
    if (_SIM800_TX_BUFF_HEAD != _SIM800_TX_BUFF_TAIL) {
        uint8_t c = _SIM800_TX_BUFF[_SIM800_TX_BUFF_TAIL];
        if (++_SIM800_TX_BUFF_TAIL >= GSM_TX_BUFF_SIZE) _SIM800_TX_BUFF_TAIL = 0; 
        UDR1 = c;
    }
    
}

#endif

uint16_t _hal_modem_tx_buff_size() { return sizeof(_SIM800_TX_BUFF); }
uint16_t _hal_modem_rx_buff_size() { return sizeof(Rx_BUF); }
void *_hal_modem_tx_buff_ptr() { return (void *)&_SIM800_TX_BUFF; }
void *_hal_modem_rx_buff_ptr() { return (void *)&Rx_BUF; }

}


