// /*
// #include "spimaster.h"
// #include "pins.h"
// 
// 
// namespace mcu {
// 
//     SpiMaster::SpiMaster( mcu::Pin ss_pin,
//                           uint8_t dataMode,
//                           uint8_t clockDiv,
//                           uint8_t bitOrder):
//                           ss_pin(ss_pin),
//                           MOSI_pin(PIN_MOSI),
//                           MISO_pin(PIN_MISO),
//                           SCK_pin(PIN_SCK)
// {
// 
//     ss_pin = true;
// 
// //     CS_pin.init(ss_port, ss_pin, DigitalPin::OUT);
// //     CS_pin.writeHigh();
// 
// //     MOSI_pin.init(MOSI_REG, MOSI_IDX,   DigitalPin::OUT);
// //     MISO_pin.init(MISO_REG, MISO_IDX,   DigitalPin::INP_PU);
// //     SCK_pin.init (SCK_REG,  SCK_IDX,    DigitalPin::OUT);
// 
//     // Invert the SPI2X bit
//     clockDiv ^= 0x1;
// 
//     // Pack into the SPISettings class
// //     reg_SPCR = _BV(SPE) | _BV(MSTR) | ((bitOrder == LSBFIRST) ? _BV(DORD) : 0) |
// //     (dataMode & SPI_MODE_MASK) | ((clockDiv >> 1) & SPI_CLOCK_MASK);
// //     reg_SPSR = clockDiv & SPI_2XCLOCK_MASK;
// 
// //     DDRB |= _BV(DDB5) | _BV(DDB7);
// //     DDRB &= ~(_BV(DDB6));
// }
// 
// 
// void SpiMaster::startTransmission() {
// //     SPSR = reg_SPSR;
// //     SPCR = reg_SPCR;
// //     ss_pin = 0;
// }
// 
// void SpiMaster::endTransmission() {
// //     SPCR &= ~(_BV(SPE));
// //     ss_pin = 1;
// }
// 
// uint8_t SpiMaster::transfer(uint8_t data) {
// //     SPDR = data;
// //     while (!(SPSR & _BV(SPIF)));
// //     return SPDR;
// }
// 
// uint8_t SpiMaster::transfer() {
// //     SPDR = 0;
// //     while (!(SPSR & _BV(SPIF)));
// //     return SPDR;
// }
// 
// void SpiMaster::transfer(void *buf, uint8_t count) {
//     if (count == 0) return;
// //     uint8_t *p = (uint8_t *)buf;
// //     SPDR = *p;
// //     while (--count > 0) {
// //         uint8_t out = *(p + 1);
// //         while (!(SPSR & _BV(SPIF)));
// //         uint8_t in = SPDR;
// //         SPDR = out;
// //         *p++ = in;
// //     }
// //     while (!(SPSR & _BV(SPIF)));
// //     *p = SPDR;
// }
// 
// uint16_t SpiMaster::transfer16(uint16_t data) {
//     union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
// //     in.val = data;
// //     if (!(SPCR & _BV(DORD))) {
// //         SPDR = in.msb;
// //         while (!(SPSR & _BV(SPIF)));
// //         out.msb = SPDR;
// //         SPDR = in.lsb;
// //         while (!(SPSR & _BV(SPIF)));
// //         out.lsb = SPDR;
// //     } else {
// //         SPDR = in.lsb;
// //         while (!(SPSR & _BV(SPIF)));
// //         out.lsb = SPDR;
// //         SPDR = in.msb;
// //         while (!(SPSR & _BV(SPIF)));
// //         out.msb = SPDR;
// //     }
//     return out.val;
// }
// 
// } // namespace devices*/
