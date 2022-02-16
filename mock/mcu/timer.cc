#include "mcu/mcu.h"
#include "mcu/timer.h"
#include "utils/atomic.h"

long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

uint32_t millis_m()
{
    static int       runningTheFirstTime_flag = 1;
    static uint64_t  startTime_msec;
    uint32_t         t;
    
    if( runningTheFirstTime_flag )
    {
        runningTheFirstTime_flag = 0;
        t = 0;
        startTime_msec = current_timestamp();
    }else
    {
        t = current_timestamp() - startTime_msec;
    }
    return t;
}

uint32_t micros()
{
    return millis_m()*1000;
}

void delayMicroseconds(uint32_t t)
{
    usleep(t);
}

void delay(uint32_t t)
{
    usleep( (uint64_t)t *1000);
}

void yield()
{
    
}





namespace mcu {

    uint32_t Timer::millis() {
        static mcu::Timer sysTime;
        return sysTime.millis_impl();
    }
        
    Timer::Timer() {
        SEI;
        // regs init
    }
    
    uint32_t Timer::millis_impl() const {
        utils::Atomic _atomic;
        return millis_m();
    }
    
}  // namespace system

