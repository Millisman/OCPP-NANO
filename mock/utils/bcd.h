#pragma once

#include <stdint.h>

// namespace {

// // // // constexpr char nibble[16] = {
// // // //     '0', '1', '2', '3', '4',
// // // //     '5', '6', '7', '8', '9',
// // // //     'A', 'B', 'C', 'D', 'E',
// // // //     'F',
// // // // };
// // // // 
// // // // // #define ToHex(T) (char)nibble[ T >> 4 ] << (char)nibble[ T & 0x0F ]
// // // // // 
// // // // // }



namespace utils {

uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(const uint8_t bcd);
    
}  // namespace utils

