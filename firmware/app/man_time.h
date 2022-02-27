#pragma once

#include "event/handler.h"
#include "utils/cpp.h"
#include "events.h"
#include "event/loop.h"
#include "utils/crc.h"
#include "utils/rtc_lib.h"
#include "mcu/timer.h"
#include "stream/uart0stream.h"
#include "store/settings.h"


namespace app {

class ManTime:  public event::Handler {
    stream::UartStream usbser;
    store::EepromSettings &settings;
    
public:
    
    ManTime();
    
    static ManTime& init() {
        static ManTime mTime;
        return mTime;
    }
    
private:

    bool lastCalc;
    uint32_t lastMillis;
    uint32_t lastUnix;
    uint16_t save_interval;
    void calc_now();
    
    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManTime);
};

}
