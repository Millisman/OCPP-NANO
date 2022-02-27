#include "timer.h"
#include "utils/atomic.h"

#ifdef __AVR__
namespace {
// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW ((64 * 256) / (F_CPU / 1000000UL))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000UL)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile uint32_t timer0_millis = 0;
// volatile uint8_t timer0_gfi_var = 0;
uint8_t timer0_fract = 0;

ISR(TIMER0_OVF_vect) {
    //     timer0_gfi_var++;
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = timer0_millis + MILLIS_INC;
    uint8_t f = timer0_fract + FRACT_INC;
    
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        ++m;
    }
    
    timer0_fract = f;
    timer0_millis = m;
}
}  // namespace
#else

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

uint32_t millis_m() {
    static int       runningTheFirstTime_flag = 1;
    static uint64_t  startTime_msec;
    uint32_t         t;
    if( runningTheFirstTime_flag ) {
        runningTheFirstTime_flag = 0;
        t = 0;
        startTime_msec = current_timestamp();
    } else {
        t = current_timestamp() - startTime_msec;
    }
    return t;
}

uint32_t micros() { return millis_m()*1000; }
void delayMicroseconds(uint32_t t) { usleep(t); }
void delay(uint32_t t) { usleep( (uint64_t)t *1000); }


#endif







namespace mcu {

    uint32_t Timer::millis() {
        static mcu::Timer sysTime;
        return sysTime.millis_impl();
    }
        
    Timer::Timer() {
        SEI;

#ifdef __AVR__
        // Fast PWM mode; OCRx update BOTTOM, TOV at MAX
        TCCR0A |= _BV(WGM01) | _BV(WGM00);
        // Set pre-scale factor to 64
        TCCR0B |= _BV(CS01) | _BV(CS00);
        // Enable timer 0 overflow interrupt
        TIMSK0 |= _BV(TOIE0);
#endif
        
    }
    
    uint32_t Timer::millis_impl() const {
        utils::Atomic _atomic;
#ifdef __AVR__
        return timer0_millis;
#else
        return millis_m();
#endif
    }
    
}  // namespace system

