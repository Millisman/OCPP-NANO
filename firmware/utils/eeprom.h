#pragma once

#include "mcu/mcu.h"


namespace utils {
    
    template <class T>
    class Eeprom {
    public:
        static T load(const uint8_t offset) {
            T t;
            eeprom_read_block(&t, reinterpret_cast<const void *>(offset), sizeof(T));
            return t;
        }
        
        static void save(const uint8_t offset, const T &data) {
            eeprom_write_block(&data, reinterpret_cast<const void *>(offset),
                               sizeof(T));
        }
    };
    
}  // namespace utils

