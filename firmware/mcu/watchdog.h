#pragma once
#include "mcu/mcu.h"
#include "utils/cpp.h"


namespace mcu {
    
    class Watchdog {
        Watchdog();
        
    public:
        static void enable();
        static void reset();
        static void forceRestart();
        
    private:
        DISALLOW_COPY_AND_ASSIGN(Watchdog);
    };
    
}  // namespace system

