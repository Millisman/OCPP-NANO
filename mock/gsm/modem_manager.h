#pragma once
#include "mcu/mcu.h"

#include "event/handler.h"
#include "utils/cpp.h"
#include "mcu/pin.h"
#include "mcu/timer.h"
#include "pins.h"
#include "events.h"
#include "event/loop.h"
#include "store/settings.h"

#ifdef __AVR__
typedef void (*GsmRecvHook)(uint8_t *payload, uint16_t length);
#else
typedef std::function<void(uint8_t *payload, uint16_t length)> GsmRecvHook;
#endif

namespace gsm {

class ManGSM : public event::Handler, public stream::OutputStream {
    stream::UartStream cout;
    bool m_Debug; // debug
    uint16_t send_schedule_size;
    GsmRecvHook on_Recv = NULL;
    int sock_fd;
    uint8_t recv_buff[512];
    uint8_t send_buff[512];
    size_t m_Readed;
    
    uint16_t m_Writted;
    bool m_Connected;
    bool m_Busy;
    
public:
    static ManGSM& init() { static ManGSM man_gsm; return man_gsm; }
    bool is_Connected() { return (sock_fd > 0); } // connection complete
    bool is_Busy() { return false; }     // modem not ready, or blocking state

    bool send_Schedule(uint16_t size) {
        cout << "\nsend_schedule=" << size << EOL;
        memset(send_buff, 0, sizeof(send_buff));
        send_schedule_size = size;
        m_Writted = 0;
        return true;
    }

    void set_RecvHook(GsmRecvHook f) { on_Recv = f; }
    
    void write(char c) override;


    
private:
    ManGSM();

    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManGSM);
};


}  // namespace gsm

