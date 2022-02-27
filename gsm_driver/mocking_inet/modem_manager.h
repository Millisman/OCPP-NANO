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
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

#include "../../secret.h"

#define LOG_ON

#ifdef LOG_ON
#define LOGGING(x) cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif

namespace gsm {

using stream::PGM;

typedef void (*GsmRecvHook)(uint8_t *payload, uint16_t length);

class ManGSM : public event::Handler, public stream::OutputStream {
    uint8_t *rx_buf;
    uint8_t *tx_buf;
    uint16_t rx_sz;
    uint16_t tx_sz;
    stream::UartStream cout;
    GsmRecvHook on_Recv = NULL;
    uint16_t m_Writted;
    uint16_t m_Readed;
    uint16_t send_schedule_size;
    bool m_Connected;
    bool m_Busy;
    bool m_Debug;
    int sock_fd;
    void start_connect();
public:
    
    static ManGSM &get() { static ManGSM man; return man; }
    
    static void set_buf(void *rp, void *tp, uint16_t rs, uint16_t ts) {
        ManGSM &man = get();
        man.rx_buf = (uint8_t *)rp;
        man.tx_buf = (uint8_t *)tp;
        man.rx_sz = rs;
        man.tx_sz = ts;
        man.start_connect();
    }
   
    void write(char c) override;
    
    bool  send_Schedule(uint16_t size) {
        m_Writted = 0;
        if (m_Busy == false) {
            send_schedule_size = size;
            return true;
        }
        return false;
    }
    
    void send_msg( const char* cmd ) {
        if (cmd) while (*cmd) { write(*cmd++); }
        write(26);
    }
    
    void send_raw(const char* raw, uint16_t size) {
        if (raw) {
            for(uint16_t i =0; i < size; i++) { write(*raw++); }
        }
    }
    
    void send_string(const char* str) {
        if (str) while (*str) { write(*str++); }
    }
    
    void send_string_P(const char* str) {
        if (str) {
            char c;
            while ((c = pgm_read_byte(str++))) { write(c); }
        }
    }
    
    void set_RecvHook(GsmRecvHook f) { on_Recv = f; }
    bool is_Connected() { return m_Connected; } // connection complete
    bool is_Busy() { return m_Busy; }     // modem not ready, or blocking state

private:
    ManGSM();

    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManGSM);
};


}  // namespace gsm
