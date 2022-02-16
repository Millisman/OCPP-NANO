#pragma once

#include <stdint.h>

#include "outputstream.h"

namespace stream {
    
    class Time {
    public:
        Time(const uint8_t hh, const uint8_t mm);
        
    private:
        uint8_t hh;
        uint8_t mm;
        friend OutputStream &operator<<(OutputStream &out, const Time &time);
    };
    
    OutputStream &operator<<(OutputStream &out, const Time &time);
    
}  // namespace stream
