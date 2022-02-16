// // #pragma once
// // 
// // #include <stdint.h>
// // 
// // #include "utils/cpp.h"
// // #include "mcu/pin.h"
// // 
// // namespace mcu {
// //     
// // #ifndef LSBFIRST
// // #define LSBFIRST 0
// // #endif
// // #ifndef MSBFIRST
// // #define MSBFIRST 1
// // #endif
// // 
// // #define SPI_CLOCK_DIV4      0x00
// // #define SPI_CLOCK_DIV16     0x01
// // #define SPI_CLOCK_DIV64     0x02
// // #define SPI_CLOCK_DIV128    0x03
// // #define SPI_CLOCK_DIV2      0x04
// // #define SPI_CLOCK_DIV8      0x05
// // #define SPI_CLOCK_DIV32     0x06
// // 
// // #define SPI_MODE0   0x00
// // #define SPI_MODE1   0x04
// // #define SPI_MODE2   0x08
// // #define SPI_MODE3   0x0C
// // 
// // #define SPI_MODE_MASK       0x0C      // CPOL = bit 3, CPHA = bit 2 on SPCR
// // #define SPI_CLOCK_MASK      0x03     // SPR1 = bit 1, SPR0 = bit 0 on SPCR
// // #define SPI_2XCLOCK_MASK    0x01   // SPI2X = bit 0 on SPSR
// // 
// // class SpiMaster {
// // 
// // public:
// //     SpiMaster(mcu::Pin ss_pin,
// //              uint8_t dataMode,
// //              uint8_t clockDiv,
// //              uint8_t bitOrder);
// //     
// //     void startTransmission();
// //     void endTransmission();
// //     uint8_t transfer(uint8_t data);
// //     uint8_t transfer();
// //     void transfer(void *buf, uint8_t count);
// //     uint16_t transfer16(uint16_t data);
// //     
// //     
// // private:
// //     uint8_t reg_SPCR;
// //     uint8_t reg_SPSR;
// //     
// //     mcu::Pin ss_pin;
// //     mcu::Pin MOSI_pin;
// //     mcu::Pin MISO_pin;
// //     mcu::Pin SCK_pin;
// // };
// // 
// // 
// // } // namespace devices
