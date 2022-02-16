#include "crc.h"


namespace utils {


uint8_t gencrc8(uint8_t *data, uint16_t len) {
    uint8_t crc = 0xff;
    uint16_t i, j;
    for (i = 0; i < len; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x31);
            else crc <<= 1;
        }
    }
    return crc;
}




}
