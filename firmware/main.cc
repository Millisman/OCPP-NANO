#include "mcu/mcu.h"
#include "mcu/watchdog.h"
#include "mcu/serial.h"
#include "gsm/modem_manager.h"
#include "ocpp/ocpp_man.h"
#include "app/man_time.h"
#include "event/loop.h"
#include "events.h"

static uint8_t rx_buff[512];
static uint8_t tx_buff[512];


int main() {
    //std::cout.setf(std::ios::unitbuf);
    SEI;
    mcu::Watchdog::enable();
    app::ManTime::init();
    
    gsm::ManGSM::get();
    gsm::ManGSM::set_buf(rx_buff, tx_buff, sizeof(rx_buff), sizeof(tx_buff));
    
    ocpp::ManOCPP::get();
    ocpp::ManOCPP::set_buf(rx_buff, tx_buff, sizeof(rx_buff), sizeof(tx_buff));
    
    while (1) {
        event::Loop::post(event::Event(events::UPDATE));
        event::Loop::dispatch();
        mcu::Watchdog::reset();
        
    }
}
 
