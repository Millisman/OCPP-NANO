#pragma once
#include "mcu/mcu.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#define memcpy_P memcpy
#endif

#define HASH_LENGTH     20
#define BLOCK_LENGTH    64

namespace ocpp {

typedef struct __attribute__((__packed__)) {
    
    union {
        uint8_t     b[BLOCK_LENGTH];
        uint32_t    w[BLOCK_LENGTH/4];
    } buffer;
    
    union {
        uint8_t     b[HASH_LENGTH];
        uint32_t    w[HASH_LENGTH/4];
    } state;
    
    uint8_t keyBuffer[BLOCK_LENGTH];
    uint8_t innerHash[HASH_LENGTH];

} sha1_mem;

const uint8_t sha1InitState[] PROGMEM = {
    0x01,0x23,0x45,0x67, // H0
    0x89,0xab,0xcd,0xef, // H1
    0xfe,0xdc,0xba,0x98, // H2
    0x76,0x54,0x32,0x10, // H3
    0xf0,0xe1,0xd2,0xc3  // H4
};

class Sha1Class {
    sha1_mem *mem;
    void pad();
    void addUncounted(uint8_t data);
    void hashBlock();
    uint32_t rol32(uint32_t number, uint8_t bits);
    // -------------------------------------
    uint8_t bufferOffset;
    uint32_t byteCount;
    // -----------------------------------
public:
    void set_m(sha1_mem *m);
    void init();
    void initHmac(const uint8_t* secret, int secretLength);
    uint8_t *result(void);
    uint8_t *resultHmac(void);
    virtual size_t write(uint8_t);
};

}
