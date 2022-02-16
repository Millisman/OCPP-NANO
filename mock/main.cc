#include "mcu/mcu.h"
#include "mcu/watchdog.h"
#include "mcu/serial.h"
#include "event/loop.h"
#include "events.h"
#include "gsm/modem_manager.h"
#include "ocpp/ocpp_man.h"
#include "app/man_time.h"

int main() {
    std::cout.setf(std::ios::unitbuf);
    SEI;
    mcu::Watchdog::enable();
    app::ManTime::init();
    gsm::ManGSM::init();
    ocpp::ManOCPP::init();
    while (1) {
        event::Loop::post(event::Event(events::UPDATE));
        event::Loop::dispatch();
        mcu::Watchdog::reset();
    }
}
 
