#include "bcd.h"


namespace utils {
    
    uint8_t dec2bcd(uint8_t dec) {
        uint8_t bcd = 0;
        while (dec >= 10) {
            bcd += 0x10;
            dec -= 10;
        }
        return bcd | dec;
    }
    
    uint8_t bcd2dec(const uint8_t bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0F); }
    
}
