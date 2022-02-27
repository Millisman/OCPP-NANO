#include "watchdog.h"

namespace mcu {
    
#ifdef __AVR__
void Watchdog::enable() { wdt_enable(WDTO_4S); }
void Watchdog::reset() { wdt_reset(); }
void Watchdog::forceRestart() {
    wdt_enable(WDTO_15MS);
    while (1) { }
}
#else
void Watchdog::enable() { }
void Watchdog::reset() { usleep(5000);  }
void Watchdog::forceRestart() {
    while (1) { }
}
#endif
    
}  // namespace mcu
