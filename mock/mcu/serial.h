#pragma once
#include "mcu/mcu.h"
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include "utils/cpp.h"





#define  SERIAL_5N1  0x00
#define  SERIAL_6N1  0x02
#define  SERIAL_7N1  0x04
#define  SERIAL_8N1  0x06
#define  SERIAL_5N2  0x08
#define  SERIAL_6N2  0x0A
#define  SERIAL_7N2  0x0C
#define  SERIAL_8N2  0x0E
#define  SERIAL_5E1  0x20
#define  SERIAL_6E1  0x22
#define  SERIAL_7E1  0x24
#define  SERIAL_8E1  0x26
#define  SERIAL_5E2  0x28
#define  SERIAL_6E2  0x2A
#define  SERIAL_7E2  0x2C
#define  SERIAL_8E2  0x2E
#define  SERIAL_5O1  0x30
#define  SERIAL_6O1  0x32
#define  SERIAL_7O1  0x34
#define  SERIAL_8O1  0x36
#define  SERIAL_5O2  0x38
#define  SERIAL_6O2  0x3A
#define  SERIAL_7O2  0x3C
#define  SERIAL_8O2  0x3E



#ifndef SERIAL0_BAUD
#define SERIAL0_BAUD 115200
#endif

#ifndef SERIAL1_BAUD
#define SERIAL1_BAUD 57600
#endif

#ifndef SERIAL2_BAUD
#define SERIAL2_BAUD 9600
#endif

#ifndef SERIAL3_BAUD
#define SERIAL3_BAUD 9600
#endif


#ifndef SERIAL0_CONFIG
#define SERIAL0_CONFIG SERIAL_8N1
#endif

#ifndef SERIAL1_CONFIG
#define SERIAL1_CONFIG SERIAL_8N1
#endif

#ifndef SERIAL2_CONFIG
#define SERIAL2_CONFIG SERIAL_8N1
#endif

#ifndef SERIAL3_CONFIG
#define SERIAL3_CONFIG SERIAL_8N1
#endif

#ifndef SERIAL0_TX_BUFFER_SIZE
#define SERIAL0_TX_BUFFER_SIZE 64
#endif

#ifndef SERIAL0_RX_BUFFER_SIZE
#define SERIAL0_RX_BUFFER_SIZE 64
#endif

#ifndef SERIAL1_TX_BUFFER_SIZE
#define SERIAL1_TX_BUFFER_SIZE 64
#endif

#ifndef SERIAL1_RX_BUFFER_SIZE
#define SERIAL1_RX_BUFFER_SIZE 64
#endif

#ifndef SERIAL2_TX_BUFFER_SIZE
#define SERIAL2_TX_BUFFER_SIZE 64
#endif

#ifndef SERIAL2_RX_BUFFER_SIZE
#define SERIAL2_RX_BUFFER_SIZE 64
#endif

#ifndef SERIAL3_TX_BUFFER_SIZE
#define SERIAL3_TX_BUFFER_SIZE 64
#endif

#ifndef SERIAL3_RX_BUFFER_SIZE
#define SERIAL3_RX_BUFFER_SIZE 64
#endif

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define bit(b) (1UL << (b))


namespace mcu {

class HardwareSerial {
public:
    HardwareSerial() {}
    virtual ~HardwareSerial() {}
    HardwareSerial &operator<<(const uint8_t &ch) { write(ch); return *this; }
    HardwareSerial &operator<<(const char *str) {while (*str) write(*str++); return *this; }
    virtual void write(uint8_t) = 0;
protected:

};

class Serial0 : public HardwareSerial {
public:
    Serial0();
    
    static Serial0 &init() {
        static Serial0 usart;
        return usart;
    }
    void write(uint8_t) override;
private:
    DISALLOW_COPY_AND_ASSIGN(Serial0);
protected:
//     SERIAL0_WRITTEN(bool _written;)
};

}
