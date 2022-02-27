#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace ocpp {

//UUID structure defined as mentioned in RFC4122
struct uuid_RFC4122
{
    uint32_t     time_low;
    uint16_t     time_mid;
    uint16_t     time_hi_and_version;
    uint16_t     clock_seq;
    uint8_t      node[6];
};

typedef uint8_t uuid_bytes_t[16];
void __uuid_generate_random(uuid_bytes_t out, uint16_t* num);
void uuid_unpack(const uuid_bytes_t in, struct uuid_RFC4122 *uu);
void uuid_pack(const struct uuid_RFC4122 *uu, uuid_bytes_t ptr);

}
