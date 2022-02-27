#pragma once
#include "event/handler.h"
#include "utils/cpp.h"
#include "mcu/pin.h"
#include "mcu/timer.h"
#include "pins.h"
#include "events.h"
#include "event/loop.h"
#include "store/settings.h"
#include "gsm/modem_manager.h"
#include "ocpp_parser.h"
#include "../../secret.h"
#include "sha1.h"

#define STACK_SIZE 4

namespace ocpp {

typedef struct __attribute__((__packed__)) {
    char KeyStart[30];
    char Key[25];
} ws_key_struct;


enum ConnStates {Conn_IDLE, Conn_SEND_SIZE, Conn_SEND_DATA};

class ManOCPP : public event::Handler {
    stream::UartStream cout;
    gsm::ManGSM &gs_man;
    MaskingStream ms;
    OCPP_Parser ocpp_parser;
    Sha1Class Sha1;
    uint32_t m_nov, m_prev;
    uint32_t m_nov_UT, m_prev_UT, moment;
    ws_key_struct ws_key;
    ConnStates mConnState = Conn_IDLE;
    
    bool message_wait;
    uint8_t message_payload;
    uint16_t message_length;
    uint16_t counter_Heartbeat = 5; // TODO set intervals
    uint16_t counter_Authorize = 5;   // example
    
    uint16_t counter_s1 = 10;
    uint16_t counter_s2 = 20;
    
    
    
    uint16_t counter_StatusNotification = 5;
    bool online;

    uint8_t *rx_buf;
    uint8_t *tx_buf;
    uint16_t rx_sz;
    uint16_t tx_sz;
    
public:
    bool onread(uint8_t *payload, uint16_t length);
    void  update();

    static ManOCPP &get() {
        static ManOCPP man;
        return man;
    }
    
    static void set_buf(void *rp, void *tp, uint16_t rs, uint16_t ts) { 
        ManOCPP &man = get();
        man.rx_buf = (uint8_t *)rp;
        man.tx_buf = (uint8_t *)tp;
        man.rx_sz = rs;
        man.tx_sz = ts;
    }
    
    static void Recv(uint8_t *payload, uint16_t length) {
        if (payload && length) {
            get().onread(payload, length);
        }
    }
    
private:
    ManOCPP();
    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManOCPP);
    uint8_t stack_count;
    OCPP_States contents[STACK_SIZE];    
    OCPP_States stack_pop();
    void stack_push(const OCPP_States c);
    void write_Starting(OCPP_States state);
    void writeCompletion();
    OCPP_States OCPP_Current_State_Req;
    uint16_t to_server;
};

}  // namespace ocpp

