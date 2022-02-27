#include "uuid.h"

namespace ocpp {

void __uuid_generate_random(uuid_bytes_t out, uint16_t *num) {
    uuid_bytes_t buf;
    struct uuid_RFC4122 uu;
    uint16_t n;
    if (!num || !*num) n = 1;
    else n = *num;
    for (uint16_t i = 0; i < n; i++) {
        for (uint8_t x = 0; x < sizeof(buf); ++x) {
            buf[x] = rand() % 256;
        }
        // random_get_bytes(buf, sizeof(buf));
        uuid_unpack(buf, &uu);
        uu.clock_seq = (uu.clock_seq & 0x3FFF) | 0x8000;
        uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF) | 0x4000;
        uuid_pack(&uu, out);
        out += sizeof(uuid_bytes_t);
    }
}
void uuid_unpack(const uuid_bytes_t in, struct uuid_RFC4122 *uu) {
    const uint8_t   *ptr = in;
    uint32_t        tmp;
    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_low = tmp;
    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_mid = tmp;
    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->time_hi_and_version = tmp;
    tmp = *ptr++;
    tmp = (tmp << 8) | *ptr++;
    uu->clock_seq = tmp;
    memcpy(uu->node, ptr, 6);
}
void uuid_pack(const struct uuid_RFC4122 *uu, uuid_bytes_t ptr) {
    uint32_t   tmp;
    uint8_t   *out = ptr;
    tmp = uu->time_low;     
    out[3] = (uint8_t)tmp; tmp >>= 8;
    out[2] = (uint8_t)tmp; tmp >>= 8;
    out[1] = (uint8_t)tmp; tmp >>= 8;
    out[0] = (uint8_t)tmp;
    tmp = uu->time_mid;
    out[5] = (uint8_t)tmp; tmp >>= 8;
    out[4] = (uint8_t)tmp;
    tmp = uu->time_hi_and_version;
    out[7] = (uint8_t) tmp; tmp >>= 8;
    out[6] = (uint8_t) tmp;
    tmp = uu->clock_seq;
    out[9] = (uint8_t) tmp; tmp >>= 8;
    out[8] = (uint8_t) tmp;
    memcpy(out + 10, uu->node, 6);
}

}
