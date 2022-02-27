#include "mcu/mcu.h"
#include "app/man_time.h"


#define SAVE_INTERVAL 3600U

namespace app {

ManTime::ManTime():
    usbser(mcu::Serial0::init()),
    settings(store::EepromSettings::init()),
    lastCalc(false),
    lastUnix(0),
    save_interval(SAVE_INTERVAL)
{    
    lastMillis = mcu::Timer::millis();
    usbser << stream::PGM << PSTR("Unix time init:") << settings.now() << EOL;
    
}

void ManTime::calc_now() {
    uint32_t elapsedSeconds = (mcu::Timer::millis() - lastMillis) / 1000;
    lastMillis += elapsedSeconds * 1000;
    settings.add_UnixTime(elapsedSeconds);
}

void ManTime::onEvent(const event::Event& event) {
    if (event.id == events::UPDATE) {
        if (lastCalc) {
            if (lastUnix != settings.now()) {
                lastUnix = settings.now();
                event::Loop::post(event::Event(events::TICK_1_SEC));
                save_interval--;
                if (save_interval==0) {
                    save_interval = SAVE_INTERVAL;
                    settings.save_UnixTime();
                    usbser << EOL << stream::PGM << PSTR("UT saved:") << settings.now() << EOL;
                }
            }
            lastCalc = false;
        } else {
            calc_now();
            lastCalc = true;
        }
    }
}

}
