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
#include "modem_strings.h"
#include <stdlib.h>
#include <string.h>

#include <string.h>
#include <stdlib.h>


#define LOG_ON

#ifdef LOG_ON
#define LOGGING(x) cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif

namespace gsm {

using stream::PGM;

// typedef struct __attribute__((__packed__)) {
//     uint8_t     gsm_net_reg; // 0..5
//     uint16_t    SMS_count;
//     uint16_t    RING_count;
//     uint16_t    ALREADY_CONNECT_count;
//     uint16_t    RDY_count;
//     uint16_t    Call_Ready_count;
//     uint16_t    SMS_Ready_count;
//     uint16_t    CONNECT_OK_count;
//     uint16_t    CONNECT_FAIL_count;
//     uint16_t    SEND_OK_count;
//     uint16_t    SEND_FAIL_count;
//     uint16_t    CLOSED_count;
//     uint8_t     signal_rssi;
//     uint8_t     signal_ber;
//     uint8_t     batt_charging;
//     uint8_t     batt_level;
//     uint16_t    batt_voltage_mV;
//     int8_t      temperature; // -128; - no data
//     uint32_t    ts;
//     uint8_t     crc8; // CRC8 for data EEPROM
// } s_ModemHealth_Block;

enum SIM800_PowerEvent: uint8_t {
    NORMAL_POWER_DOWN,
    UNDER_VOLTAGE_POWER_DOWN,
    UNDER_VOLTAGE_WARNNING,
    OVER_VOLTAGE_POWER_DOWN,
    OVER_VOLTAGE_WARNNING
};
    
enum SimCPIN: uint8_t {
    CPIN_UNKNOWN = 0,
    CPIN_READY,       //MT is not pending for any password
    CPIN_SIM_PIN,     //MT is waiting SIM PIN to be given
    CPIN_SIM_PUK,     //MT is waiting for SIM PUK to be given
    CPIN_PH_SIM_PIN,  //ME is waiting for phone to SIM card (antitheft)
    CPIN_PH_SIM_PUK,  //ME is waiting for SIM PUK (antitheft)
    CPIN_SIM_PIN2,
    CPIN_SIM_PUK2
};

enum SimRegGSM: uint8_t {
    GSM_NotReg = 0,     // GSM: Not reg
    GSM_RegInHomeNet,   // GSM: Reg in home net
    GSM_NetSearching,   // GSM: Net searching...
    GSM_RegDenied,      // GSM: Reg denied
    GSM_Unknown,        // GSM: Unknown
    GSM_RegInRoaming    // GSM: Reg in Roaming
};

// Query Current Connection Status
enum SimConnStatus: uint8_t {
    CONN_UNKNOWN = 0,
    CONN_IP_INITIAL,
    CONN_INITIAL,
    CONN_IP_START,
    CONN_IP_CONFIG,
    CONN_IP_GPRSACT,
    CONN_IP_STATUS,
    CONN_TCP_CONNECTING,
    CONN_UDP_CONNECTING,
    CONN_CONNECTING,
    CONN_SERVER_LISTENING,
    CONN_LISTENING,
    CONN_CONNECT_OK,
    CONN_TCP_CLOSING,
    CONN_UDP_CLOSING,
    CONN_CLOSING,
    CONN_TCP_CLOSED,
    CONN_UDP_CLOSED,
    CONN_CLOSED,
    CONN_PDP_DEACT,
    CONN_CONNECTED,
    CONN_IP_PROCESSING,
    CONN_REMOTE_CLOSING,
    CONN_OPENING
};

enum PowerKey: uint8_t { NotPressed, Pressed, Booting }; //PressedHold
enum ModemStatus: uint8_t { UNKNOWN, DISABLED, AT_OK, GPRS_CONNECTED};

typedef void (*GsmRecvHook)(uint8_t *payload, uint16_t length);

class ManGSM : public event::Handler, public stream::OutputStream {
//     uint8_t *rx_buf;
//     uint8_t *tx_buf;
//     uint16_t rx_sz;
//     uint16_t tx_sz;
    stream::UartStream cout;
    store::EepromSettings &settings;
    bool m_Debug;
    bool        scheduled_scan;
    uint32_t    scheduled_ts_ms;
    GsmRecvHook on_Recv = NULL;
    bool flag_debug;
    char *found_in;
    bool compare_hook(const char *CmdP);
    void scan();
    bool m_Busy;
    uint16_t sched_size;
    bool qs_;
    mcu::Pin button_pin;
    PowerKey button;
    ModemStatus status;
    uint8_t istage;
    uint8_t faults;
    bool observe_GSM_OK;
    bool observe_GSM_ERROR;
    bool observe_GSM_PROMPT;
    bool observe_GSM_DATA_ACCEPT;
    uint32_t moment;
    uint32_t nov_ms;
    uint8_t gsm_mode;
    SimConnStatus ip_state;
    SimCPIN cpin_Event;
    SimRegGSM netreg_Event;
    SimConnStatus connState_Event;
    
public:
    static ManGSM &get() { static ManGSM man; return man; }
    
    static void set_buf(void *rp, void *tp, uint16_t rs, uint16_t ts);

    
    void write(char c) override;
    bool send_Schedule(uint16_t size); // <<<<<<<<<<<<<<<<<<<<<<<<
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
    
    void QuerySCIPSTATUS() { *this << stream::PGM << PSTR("AT+CIPSTATUS\r\n"); }
    void set_RecvHook(GsmRecvHook f) { on_Recv = f; }
    bool is_Connected() { return status == gsm::ModemStatus::GPRS_CONNECTED; } // connection complete
    bool is_Busy() { return m_Busy; }     // modem not ready, or blocking state
    void  update();
    void begin();
    // char *_i_ATI;  // product identification information
    // char *_i_GCAP; // complete TA capabilities list
    // char *_i_GMI;  // manufacturer identification
    // char *_i_GMM;  // TA model identification
    // char *_i_GMR;  // TA revision identification of software release
    // char *_i_GOI;  // global object identification
    // char *_i_GSN;  // TA serial number identification (IMEI)
    // char *_i_GSV;  // Product identification, manufacturer, name and revision information
    // char *_i_CIMI; // Request TA serial number identification (IMSI)
    // char *_i_CIFSR; // IP
private:
    ManGSM();
    void handle_NO_CARRIER() {
        event::Loop::post(event::Event(events::GSM_HANDLED_NO_CARRIER));
        LOGGING({ cout << PGM << PSTR("+NO CARRIER\r\n"); });
    }
    
    void handle_SEND_OK() {
        //gsm_health.SEND_OK_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_SEND_OK));
        LOGGING({ cout << PGM << PSTR("+[SEND OK]\r\n"); });
    }
    
    void handle_pCSQ() {
        uint8_t rssi = 0, ber = 0;
        char *comma1 = strchr((char*)found_in,':');
        if (comma1 != NULL) {
            comma1++;
            rssi = atoi(comma1);
            if (comma1 != NULL) {
                char *comma2 = strchr(comma1,',');
                if (comma2 != 0) {
                    comma2++;
                    ber = atoi(comma2);
                }
            }
        }
        LOGGING({
            cout << PGM << PSTR("+Signal Qty rssi: ") << rssi;
            cout << PGM << PSTR(" ber: ") << ber << EOL;
        });
        event::Loop::post(event::Event(events::GSM_HANDLED_CSQ));
        //gsm_health.signal_rssi = rssi;
        //gsm_health.signal_ber  = ber;
    }
    
    void handle_SIM800_PowerEvent(SIM800_PowerEvent pe) {
        switch (pe) {
            case NORMAL_POWER_DOWN:
                event::Loop::post(event::Event(events::GSM_NORMAL_POWER_DOWN));
                LOGGING({ cout << PGM << PSTR_NORMAL_POWER_DOWN << EOL; });
                break;
            case UNDER_VOLTAGE_POWER_DOWN:
                event::Loop::post(event::Event(events::GSM_UNDER_VOLTAGE_POWER_DOWN));
                LOGGING({ cout << PGM << PSTR_UNDER_VOLTAGE_POWER_DOWN << EOL; });
                break;
            case UNDER_VOLTAGE_WARNNING:
                event::Loop::post(event::Event(events::GSM_UNDER_VOLTAGE_WARNNING));
                LOGGING({ cout << PGM << PSTR_UNDER_VOLTAGE_WARNNING << EOL; });
                break;
            case OVER_VOLTAGE_POWER_DOWN:
                event::Loop::post(event::Event(events::GSM_OVER_VOLTAGE_POWER_DOWN));
                LOGGING({ cout << PGM << PSTR_OVER_VOLTAGE_POWER_DOWN << EOL; });
                break;
            case OVER_VOLTAGE_WARNNING:
                event::Loop::post(event::Event(events::GSM_OVER_VOLTAGE_WARNNING));
                LOGGING({ cout << PGM << PSTR_OVER_VOLTAGE_WARNNING << EOL; });
                break;
        }
    }
    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManGSM);
};


}  // namespace gsm
