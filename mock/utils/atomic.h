#pragma once

#include "mcu/mcu.h"

namespace utils {

class Atomic {
    uint8_t sreg;
public:
#ifdef __AVR__
    __attribute__((always_inline)) inline Atomic() { sreg = SREG; cli(); }
    __attribute__((always_inline)) inline ~Atomic() { SREG = sreg; sei(); }
#else
    __attribute__((always_inline)) inline Atomic() { }
    __attribute__((always_inline)) inline ~Atomic() { }
#endif
};

}  // namespace utils

