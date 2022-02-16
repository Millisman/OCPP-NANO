
#pragma once

#include <stdint.h>


namespace mcu {
    
    // Simple tick counter for various timing tasks.
    // Using this will enable global interrupts and use
    // timer0.
    class Timer {
        Timer();
        
    public:
        // This is lazily initialized. First call will always
        // return 0. TImer overflows every ~49 days.
        static uint32_t millis();
        static uint8_t gfi_cnt();
        
    private:
        uint32_t millis_impl() const;
    };
    
}  // namespace mcu

