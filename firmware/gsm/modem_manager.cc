#include "mcu/mcu.h"
#include "modem_manager.h"
#include "modem_hal.h"

namespace gsm {

    
ManGSM::ManGSM():
    cout(mcu::Serial0::init())
    , settings(store::EepromSettings::init())
    , m_Debug(true)
    , button_pin(PIN_GSM_POWER)
    , scheduled_scan(false)
    , flag_debug(true)
    , ip_state(CONN_UNKNOWN)
    , cpin_Event(CPIN_UNKNOWN)
    , on_Recv(NULL)

{
    cout << stream::PGM << PSTR("GSM driver ");
    _hal_modem_init_port();
    gsm_mode = settings.get_device_param(store::P_GSM_MOD); 
    if (gsm_mode) {
        cout << stream::PGM << PSTR("enabled: ") << gsm_mode << EOL;
        begin();
    } else {
        button_pin = 0;
        cout << stream::PGM << PSTR("disabled\r\n");
        button = gsm::PowerKey::NotPressed;
        status = ModemStatus::DISABLED;
    }
    scheduled_scan = false;
    scheduled_ts_ms = 0;
}

void ManGSM::write(const char c) {
    _hal_modem_write(c);
    if (flag_debug) { cout << (char)c; }
}

void ManGSM::set_buf(void *rp, void *tp, uint16_t rs, uint16_t ts) {
    GSM_RX_BUFF_SIZE = rs;
    GSM_TX_BUFF_SIZE = ts;
    _SIM800_TX_BUFF = (uint8_t *)tp;
    Rx_BUF = (uint8_t *)rp;
}





//if (status != gsm::ModemStatus::DISABLED) update();
void ManGSM::onEvent(const event::Event& event) {
    // if (status == gsm::ModemStatus::DISABLED) return;
#ifdef __linux__
    service_tty();
#endif    
    nov_ms = mcu::Timer::millis();
    
    switch ((events)event.id) {
        
        case events::UPDATE:
            
            
            
            if (RxOF) {
                RxOF = false;
                Rx_POS = 0;
                if (flag_debug) { cout << PGM << PSTR("GSM: BUFF OVF\r\n"); }
            }
            if (RxOK) {
                RxOK = false;
                observe_GSM_OK = true;
                if (flag_debug) { cout << PGM << PSTR("GSM: OK\r\n"); }
            }
            if (RxER) {
                RxER = false;
                observe_GSM_ERROR = true;
                if (flag_debug) { cout << PGM << PSTR("GSM: ERR\r\n"); }
            }
            if (RxPP) {
                RxPP = false;
                Rx_POS = 0;
                observe_GSM_PROMPT = true;
                if (flag_debug) { cout << PGM << PSTR("GSM: PROMPT\r\n"); }
            }
            if (RxSC) {
                RxSC = false;
                scheduled_ts_ms = nov_ms;
                scheduled_scan = true;
            }
            if (scheduled_scan) {
                if (nov_ms - scheduled_ts_ms > 200) {
                    scheduled_scan = false;
                    if (flag_debug) { cout << PGM << PSTR("GSM: SCAN\r\n"); }
                    scan();
                    Rx_POS = 0;
                }
            }
            // if (status == gsm::ModemStatus::DISABLED) return;
            update(); ///<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        break;
        
        case events::GSM_HANDLED_CLOSED:
            if (status == ModemStatus::GPRS_CONNECTED) {
                status = ModemStatus::AT_OK; // TODO rotate servers
                moment = nov_ms;
                istage = 0;
            }
            break;
            
        case events::GSM_HANDLED_STATE:
            connState_Event = (SimConnStatus)event.param;
            if ((connState_Event == SimConnStatus::CONN_TCP_CLOSED) ||
                (connState_Event == SimConnStatus::CONN_UDP_CLOSED) ||
                (connState_Event == SimConnStatus::CONN_CLOSED)) {
                if (status == ModemStatus::GPRS_CONNECTED) {
                    status = ModemStatus::AT_OK; // TODO rotate servers
                    moment = nov_ms;
                    istage = 0;
                }
            }
        break;
            
        case events::TICK_1_SEC:
            // TODO TOT calculation
        break;
        
        case events::GSM_MODE_SET:
            cout << stream::PGM << PSTR("Modem ");
            if (event.param == 0) {
                button_pin = 1;
                moment = nov_ms; // Startup ms...
                button = gsm::PowerKey::Pressed;
                status = ModemStatus::DISABLED;
                settings.set_device_param(store::P_GSM_MOD, 0);
                cout << stream::PGM << PSTR("OFF\r\n");
            } else {
                begin();
                settings.set_device_param(store::P_GSM_MOD, 1);
                cout << stream::PGM << PSTR("ON\r\n");
            }
        break;
        
        case GSM_DEBUG_SET:
            m_Debug = (bool)event.param;
            flag_debug = m_Debug;
        break;
            
        default: {}
    }
}

bool  ManGSM::send_Schedule(uint16_t size) {
    if (m_Busy == false) {
        sched_size = size;
        return true;
    }
    return false;
}


const char AT_CGATT[] PROGMEM = "AT+CGATT=";

void ManGSM::begin() {
    
    m_Busy = true;
    button_pin = 1;
    button = gsm::PowerKey::Pressed;
    status = gsm::ModemStatus::UNKNOWN;
    istage = 0;
    faults = 0;
    observe_GSM_OK = false;
    observe_GSM_ERROR = false;
    observe_GSM_PROMPT = false;
    observe_GSM_DATA_ACCEPT = false;
    cpin_Event = SimCPIN::CPIN_UNKNOWN;
    netreg_Event = SimRegGSM::GSM_Unknown;
    connState_Event = SimConnStatus::CONN_UNKNOWN;
    // PWRKEY should be pulled low at least 1.2 second and then released to power on/down the module.
    // User can power off SIM800 by pulling down the PWRKEY pin for at least 1.5 (<33sec)
    // second and then release.
    moment = mcu::Timer::millis(); // Startup ms...
}

void ManGSM::update() {

    nov_ms = mcu::Timer::millis();

    if (gsm::PowerKey::Pressed == button) {

        if ((nov_ms - moment) > 1600) { // 1500ms for pressing
            if (m_Debug) { cout << stream::PGM << PSTR("GSM key=0\r\n"); }
            button_pin = 0;
            button = gsm::PowerKey::Booting;
            moment = nov_ms;
        }
        
    } else if (gsm::PowerKey::Booting == button) {

        if ((nov_ms - moment) > 4500) { // 4s for booting
            button = gsm::PowerKey::NotPressed;
            moment = nov_ms;
        }
        
    } else if (gsm::PowerKey::NotPressed == button) { // testing normal mode

        if (ModemStatus::UNKNOWN == status) { // Testing AT...

            if (faults > 20) { // NO AT, disabled?
                faults = 0;
                istage = 0;
                button_pin = 1;
                if (m_Debug) { cout << stream::PGM << PSTR("GSM key=1\r\n"); }
                button = gsm::PowerKey::Pressed;
                moment = nov_ms;
                return;

            } else {
                
                
                if ((nov_ms - moment > 550)) {
                    moment = nov_ms;
                    
                    switch (istage) {
                        
                        case 0:
                            if (observe_GSM_OK) {
                                istage = 1;
                            } else {
                                *this << PGM << PSTR("AT\r\n");
                                faults++;
                            }
                        break;
                        
                        case 1:
                            if (observe_GSM_OK) {
                                istage = 2;
                            } else {
                                *this << PGM << PSTR("ATE0\r\n");
                                istage = 2;
                            }
                        break;
                            
                        case 2:
                            if (cpin_Event == SimCPIN::CPIN_READY) {
                                istage = 3;
                            } else {
                                *this << PGM << PSTR("AT+CPIN?\r\n");
                            }
                        break;
                            
                        case 3:
                            if ((netreg_Event == SimRegGSM::GSM_RegInHomeNet) ||
                                (netreg_Event == SimRegGSM::GSM_RegInRoaming) ) {
                                istage = 4;
                            } else {
                                *this << PGM << PSTR("AT+CREG?\r\n");
                            }
                        break;
                        
                        case 4:
                            // AT INIT OK ---> IP STATUS
                            status = ModemStatus::AT_OK;
                            moment = nov_ms;
                            istage = 0;
                        break;
                    }
                    Rx_POS = 0;
                    observe_GSM_OK = false;
                }
            }

        } else if (ModemStatus::AT_OK == status) {
            
            if ((nov_ms - moment > 1000)) {
                moment = nov_ms;
                qs_ = false;
                
                
                cout << '[' << istage << ']';
                
                switch (istage) {
                    // -----------------------------------
                    case 0: // prophylactic procedure
                        *this << PGM << AT_CGATT << "0\r\n";
                        istage = 1;
                    break;
                    
                    case 1: // prophylactic procedure
                        *this << PGM << PSTR("AT+CIPSHUT\r\n");
                        istage = 2;
                    break;
                        
                    case 2:
                        if (observe_GSM_OK) { istage = 3; }
                        else { *this << PGM << AT_CGATT << "1\r\n"; }
                    break;
                    // ------------------------------------
                    case 3:
                        *this << PGM << PSTR("AT+CSTT=\"internet\",\"gprs\",\"gprs\"\r\n"); // TODO
                        istage = 4;
                    break;
                        
                    case 4:
                        if (connState_Event == SimConnStatus::CONN_IP_START)
                        { istage = 6; }
                    break;

                    case 6:
                        if (connState_Event == SimConnStatus::CONN_IP_GPRSACT)
                        { istage = 7; } else { *this << PGM << PSTR("AT+CIICR\r\n"); }
                        // AT+CIPSTATUS ==> STATE: IP GPRSACT
                    break;
                        
                    case 7:
                        if (connState_Event == SimConnStatus::CONN_IP_STATUS)
                        { istage = 8; } else { *this << PGM << PSTR("AT+CIFSR\r\n"); }
                        // AT+CIPSTATUS ==> STATE: IP STATUS
                    break;
                        
                    case 8:
                        *this << PGM << PSTR("AT+CIPQSEND=1\r\n");
                        istage = 9;
                    break; // fast send
                        
                    case 9:
                        *this << PGM << PSTR("AT+CIPHEAD=1\r\n");
                        istage = 10;
                    break; // Append before data +IPD,<SIZE>:
                        
                    case 10:
                        *this << PGM << PSTR("AT+CIPSTART=\"TCP\",\"91.90.193.95\",\"80\"\r\n"); // TODO
                        istage = 11;
                    break;
                        
                    case 11:
                        if (connState_Event == SimConnStatus::CONN_CONNECT_OK) {
                            status = ModemStatus::GPRS_CONNECTED;
                            istage = 0;
                            m_Busy = false;
                        }
                    break;
                }
                
                Rx_POS = 0;
                observe_GSM_OK = false;
                qs_ = false;
            } else if ((nov_ms - moment > 500)) {
                if(!qs_) QuerySCIPSTATUS();
                qs_ = true;
            }
            
        } else if (ModemStatus::GPRS_CONNECTED == status) { // <<<<<<<<<<<<<<<<<<<<<<,
            
            switch (istage) {
                
                case 0:
                    if (sched_size > 0) {
                        moment = nov_ms;
                        m_Busy = true;
                        *this << stream::PGM << PSTR("AT+CIPSEND=") << sched_size << EOL;
                        istage = 1;
                    }
                break;
                
                case 1: // wait PROMPT
                    if ((nov_ms - moment) > 3000) {
                        if (flag_debug) { cout << PGM << PSTR("GSM: PROMPT TOT\r\n"); }
                        // m_Busy = false;
                        // istage = 0; // or reconnecting ???
                        status = ModemStatus::AT_OK;
                        moment = nov_ms;
                        istage = 0;
                    } else if (observe_GSM_PROMPT) {
                        if (flag_debug) { cout << PGM << PSTR("GSM: PROMPT ME\r\n"); }
                        moment = nov_ms;
                        m_Busy = false; // allow write
                        istage = 2;     // protocol module must write data
                    }
                break;
                
                case 2: // protocol module must write data
                
                    if (observe_GSM_DATA_ACCEPT) {
                        istage = 0; // protocol module must write data
                        sched_size = 0;
                    } else if ((nov_ms - moment) > 3000) {
                        if (flag_debug) { cout << PGM << PSTR("WAIT DATA_ACCEPT TOT, size?\r\n"); }
                        istage = 0;
                        sched_size = 0;
                    }
                break;
            
            }
        }
    }
}





bool ManGSM::compare_hook(const char *CmdP) {
    if (found_in == NULL) {
        if ((Rx_POS-1) >= strlen_P(CmdP) ) {
            found_in = strstr_P((char *)Rx_BUF, CmdP);
            if (found_in!= NULL) {
                if (flag_debug) {
                    cout << PGM << PSTR("HANDLE [") << PGM
                    << CmdP << PGM << PSTR("] POS:") << Rx_POS << EOL;
                }
            }
        }
    }
    return (found_in != NULL);
}


void ManGSM::scan() {
    
    cout << (char *)Rx_BUF << EOL;
    
    found_in = NULL;
    if (compare_hook(PSTR_STATE)) {
        if (strstr_P((char *)found_in, PSTR("IP INITIAL"))       != NULL) {ip_state = CONN_IP_INITIAL;} else
        if (strstr_P((char *)found_in, PSTR("INITIAL"))          != NULL) {ip_state = CONN_INITIAL;} else
        if (strstr_P((char *)found_in, PSTR("IP START"))         != NULL) {ip_state = CONN_IP_START;} else
        if (strstr_P((char *)found_in, PSTR("IP CONFIG"))        != NULL) {ip_state = CONN_IP_CONFIG;} else
        if (strstr_P((char *)found_in, PSTR("IP GPRSACT"))       != NULL) {ip_state = CONN_IP_GPRSACT;} else
        if (strstr_P((char *)found_in, PSTR("IP STATUS"))        != NULL) {ip_state = CONN_IP_STATUS;} else
        if (strstr_P((char *)found_in, PSTR("TCP CONNECTING"))   != NULL) {ip_state = CONN_TCP_CONNECTING;} else
        if (strstr_P((char *)found_in, PSTR("UDP CONNECTING"))   != NULL) {ip_state = CONN_UDP_CONNECTING;} else
        if (strstr_P((char *)found_in, PSTR("CONNECTING"))       != NULL) {ip_state = CONN_CONNECTING;} else
        if (strstr_P((char *)found_in, PSTR("SERVER LISTENING")) != NULL) {ip_state = CONN_SERVER_LISTENING;} else
        if (strstr_P((char *)found_in, PSTR("LISTENING"))        != NULL) {ip_state = CONN_LISTENING;} else
        if (strstr_P((char *)found_in, PSTR("CONNECT OK"))       != NULL) {ip_state = CONN_CONNECT_OK;} else
        if (strstr_P((char *)found_in, PSTR("TCP CLOSING"))      != NULL) {ip_state = CONN_TCP_CLOSING;} else
        if (strstr_P((char *)found_in, PSTR("UDP CLOSING"))      != NULL) {ip_state = CONN_UDP_CLOSING;} else
        if (strstr_P((char *)found_in, PSTR("CLOSING"))          != NULL) {ip_state = CONN_CLOSING;} else
        if (strstr_P((char *)found_in, PSTR("TCP CLOSED"))       != NULL) {ip_state = CONN_TCP_CLOSED;} else
        if (strstr_P((char *)found_in, PSTR("UDP CLOSED"))       != NULL) {ip_state = CONN_UDP_CLOSED;} else
        if (strstr_P((char *)found_in, PSTR("CLOSED"))           != NULL) {ip_state = CONN_CLOSED;} else
        if (strstr_P((char *)found_in, PSTR("PDP DEACT"))        != NULL) {ip_state = CONN_PDP_DEACT;} else
        if (strstr_P((char *)found_in, PSTR("CONNECTED"))        != NULL) {ip_state = CONN_CONNECTED;} else
        if (strstr_P((char *)found_in, PSTR("IP PROCESSING"))    != NULL) {ip_state = CONN_IP_PROCESSING;} else
        if (strstr_P((char *)found_in, PSTR("REMOTE CLOSING"))   != NULL) {ip_state = CONN_REMOTE_CLOSING;} else
        if (strstr_P((char *)found_in, PSTR("OPENING"))          != NULL) {ip_state = CONN_OPENING;}
        event::Loop::post(event::Event(events::GSM_HANDLED_STATE, ip_state));
        if (flag_debug) { cout << PGM << PSTR("IP STATE=") << (uint8_t)ip_state << EOL; }
    } else if (compare_hook(PSTR_pIPD)) {
        if (flag_debug) { cout << PGM << PSTR_pIPD << ' ' << Rx_POS; }
        uint8_t len = 0, err = 0;
        char *comma1 = strchr((char*)found_in,',');
        if (comma1 != NULL) {
            comma1++;
            if (comma1 != NULL) { //return;
                len = atoi(comma1);
                if (len <= Rx_POS) { //return; // wrong
                    if (len > 3) {
                        char *colon = strchr((char*)found_in,':');
                        colon++;
                        if (colon != NULL) { //return;
                            if (on_Recv) { on_Recv((uint8_t *)colon, Rx_POS); }
                            //protocol::Client &CL = protocol::Client::init();
                            //CL.Rx((uint8_t*)colon);
                            event::Loop::post(event::Event(events::GSM_HANDLED_IPD, Rx_POS));
                        } else err = 5;
                    } else err = 4;
                }else err = 3;
            } else err = 2;
        } else err = 1;
        if (flag_debug) { cout << PGM << PSTR_pIPD << PGM << PSTR(" P_ERR:") << err << EOL; }
    } else if (compare_hook(PSTR_DATA_ACCEPT)) {
        observe_GSM_DATA_ACCEPT = true;
        event::Loop::post(event::Event(events::GSM_HANDLED_DATA_ACCEPT));
    } else if (compare_hook(PSTR_SEND_FAIL)) {
        // gsm_health.SEND_FAIL_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_SEND_FAIL));
        LOGGING({ cout << PGM << PSTR("+[SEND FAIL]\r\n"); });
    } else if (compare_hook(PSTR_CONNECT_FAIL)) {
        // gsm_health.CONNECT_FAIL_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_CONNECT_FAIL));
        LOGGING({ cout << PGM << PSTR("+[CONNECT FAIL]\r\n"); });
    } else if (compare_hook(PSTR_pCSQ)) {
        handle_pCSQ();
    } else if (compare_hook(PSTR_pCMTE)) {
        int8_t tmp = -128;
        char *comma1 = strchr((char*)found_in,':');
        if (comma1 != NULL) {
            comma1++;
            if (comma1 != NULL) {
                char *comma2 = strchr(comma1,',');
                if (comma2 != 0) {
                    comma2++;
                    tmp = atoi(comma2);
                }
            }
        }
        event::Loop::post(event::Event(events::GSM_HANDLED_CMTE));
        LOGGING({ cout << PGM << PSTR("+Temp ") << tmp << EOL; });
        // gsm_health.temperature = tmp;
    } else if (compare_hook(PSTR_pCPIN)) {
        if      (strstr_P((char *)found_in, PSTR("READY")) != NULL)       { cpin_Event = SimCPIN::CPIN_READY; }
        else if (strstr_P((char *)found_in, PSTR("SIM PIN")) != NULL)     { cpin_Event = SimCPIN::CPIN_SIM_PIN; }
        else if (strstr_P((char *)found_in, PSTR("SIM PUK")) != NULL)     { cpin_Event = SimCPIN::CPIN_SIM_PUK; }
        else if (strstr_P((char *)found_in, PSTR("PH_SIM PIN")) != NULL)  { cpin_Event = SimCPIN::CPIN_PH_SIM_PIN; }
        else if (strstr_P((char *)found_in, PSTR("PH_SIM PUK")) != NULL)  { cpin_Event = SimCPIN::CPIN_PH_SIM_PUK; }
        else if (strstr_P((char *)found_in, PSTR("SIM PIN2")) != NULL)    { cpin_Event = SimCPIN::CPIN_SIM_PIN2; }
        else if (strstr_P((char *)found_in, PSTR("SIM PUK2")) != NULL)    { cpin_Event = SimCPIN::CPIN_SIM_PUK2; }
        LOGGING({ cout << PGM << PSTR("SimCPIN:") << (uint8_t)cpin_Event << ' ' << (char *)found_in << EOL; });
//         ccpin_Event
//         event::Loop::post(event::Event(events::GSM_HANDLED_CPIN, cpin_Event));
    } else if (compare_hook(PSTR_pGCAP)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_GCAP));
        LOGGING({ cout << PGM << PSTR("+Complete TA Capabilities List: ") << (char *)found_in << EOL; });        
    } else if (compare_hook(PSTR_pCOPS)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_COPS));
        LOGGING({ cout << PGM << PSTR("+Operator Selection: ") << (char *)found_in << EOL; });
    } else if (compare_hook(PSTR_pCSQN)) {
        handle_pCSQ();
    } else if (compare_hook(PSTR_pCREG)) {
        // Parameters <n> 0/1 Disable/Enable network registration unsolicited resul t code
        // 0 Not registered, MT is not currently searching a new operator to register to
        // 1 Registered, home network
        // 2 Not registered, but MT is currently searching a new operator to register to
        // 3 Registration denied
        // 4 Unknown
        // 5 Registered, roaming
        //         if (RxX >= 9) { //return;
        uint8_t code = 0; // uslc = 0, 
        char *comma1 = strchr((char*)found_in,':');
        if (comma1 != NULL) {
            comma1++;
            //         uslc = atoi(comma1);
            if (comma1 != NULL) {
                char *comma2 = strchr(comma1,',');
                if (comma2 != 0) {
                    comma2++;
                    code = atoi(comma2);
                }
            }
        }
        
        netreg_Event = (SimRegGSM)code;
        
        
//         // gsm_health.gsm_net_reg = code;
//         event::Loop::post(event::Event(events::GSM_HANDLED_CREG, code));
        LOGGING({
            cout << PGM << PSTR("Network Registration: ") << code << EOL;
            switch (code) {
                case 0: cout << PGM << PSTR("\tGSM: Not reg\r\n"); break;
                case 1: cout << PGM << PSTR("\tGSM: Reg in home net\r\n"); break;
                case 2: cout << PGM << PSTR("\tGSM: Net searching...\r\n"); break;
                case 3: cout << PGM << PSTR("\tGSM: Reg denied\r\n"); break;
                case 4: cout << PGM << PSTR("\tGSM: Unknown\r\n"); break;
                case 5: cout << PGM << PSTR("\tGSM: Reg in Roaming\r\n"); break;
            }
        });
    } else if (compare_hook(STATE_CLOSED)) {
        // gsm_health.CLOSED_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_CLOSED));
        LOGGING({ cout << PGM << PSTR("+[CLOSED] This is bad!\r\n"); });
    } else if (compare_hook(PSTR_RDY)) {
        // gsm_health.RDY_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_RDY));
        LOGGING({ cout << PGM << PSTR("+[RDY]\r\n"); });
    } else if (compare_hook(PSTR_pCBC)) {
        uint8_t charging = 0; uint8_t level = 0; uint16_t voltage_mV = 0;
        char *comma1 = strchr((char*)found_in,':');
        if (comma1 != NULL) {
            comma1++;
            charging = atoi(comma1);
            if (comma1 != NULL) {
                char *comma2 = strchr(comma1,',');
                if (comma2 != NULL) {
                    comma2++;
                    level = atoi(comma2);
                    if (comma2 != NULL) {
                        char *comma3 = strchr(comma2,',');
                        if (comma3 != NULL) {
                            comma3++;
                            voltage_mV = atoi(comma3);
                        }
                        
                    }
                }
            }
        }
        LOGGING({
            cout << PGM << PSTR("+Battery CRG=") << charging;
            cout << PGM << PSTR(" LEV=") << level;
            cout << PGM << PSTR(" VOLT=") << voltage_mV << EOL;
        });
        event::Loop::post(event::Event(events::GSM_HANDLED_CBC));
        // gsm_health.batt_charging = charging;
        // gsm_health.batt_level = level;
        // gsm_health.batt_voltage_mV = voltage_mV;
    } else if (compare_hook(PSTR_RING)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_RING));        
        LOGGING({ cout << PGM << PSTR("+RING\r\n"); });
        // gsm_health.RING_count++;
    } else if (compare_hook(PSTR_pCMTI)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_CMTI));
        LOGGING({ cout << PGM << PSTR("New SMS arrived: ") << (char *)found_in << EOL; });
        // gsm_health.SMS_count++;
    } else if (compare_hook(PSTR_Call_Ready)) {
        // gsm_health.Call_Ready_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_Call_Ready));
        LOGGING({ cout << PGM << PSTR("+[Call Ready]\r\n"); });
    } else if (compare_hook(PSTR_SMS_Ready)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_SMS_Ready));
        // gsm_health.SMS_Ready_count++;
        LOGGING({ cout << PGM << PSTR("+[SMS Ready]\r\n"); });
    } else if (compare_hook(PSTR_CONNECT_OK)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_CONNECT_OK));
        // gsm_health.CONNECT_OK_count++;
        LOGGING({ cout << PGM << PSTR("+[CONNECT OK]\r\n"); });  
    } else if (compare_hook(PSTR_ALREADY_CONNECT)) {
        // gsm_health.ALREADY_CONNECT_count++;
        event::Loop::post(event::Event(events::GSM_HANDLED_ALREADY_CONNECT));
        LOGGING({ cout << PGM << PSTR("+[ALREADY CONNECT]\r\n"); });
    } else if (compare_hook(PSTR_SHUT_OK)) {
        event::Loop::post(event::Event(events::GSM_HANDLED_SHUT_OK));
        LOGGING({ cout << PGM << PSTR("+[SHUT OK]\r\n"); });
    } else if (compare_hook(PSTR_NORMAL_POWER_DOWN)) {
        handle_SIM800_PowerEvent(NORMAL_POWER_DOWN);
    } else if (compare_hook(PSTR_UNDER_VOLTAGE_POWER_DOWN)) {
        handle_SIM800_PowerEvent(UNDER_VOLTAGE_POWER_DOWN);
    } else if (compare_hook(PSTR_UNDER_VOLTAGE_WARNNING)) {
        handle_SIM800_PowerEvent(UNDER_VOLTAGE_WARNNING);
    } else if (compare_hook(PSTR_OVER_VOLTAGE_POWER_DOWN)) {
        handle_SIM800_PowerEvent(OVER_VOLTAGE_POWER_DOWN);
    } else if (compare_hook(PSTR_OVER_VOLTAGE_WARNNING)) {
        handle_SIM800_PowerEvent(OVER_VOLTAGE_WARNNING);
    } else {
        //unparsed
    }
    
//     if ((found_in == NULL) && unhook_save && (Rx_POS > 4)) {
//         Rx_POS -= 4; // strip OK
//         uint8_t z = 0, j;
//         for (j = 0; j < Rx_POS; j++) {
//             if ((Rx_BUF[j] >= 0x20) && (Rx_BUF[j] <= 0x7E)) ++z;
//         }
/*        if (z > STRINFO_LEN) z = STRINFO_LEN;
        if (!unhook_ptr && z) {
            unhook_ptr = (char*)calloc(z + 1, sizeof(char));
            for (j = 0; j < Rx_POS; j++) {
                if ((Rx_BUF[j] >= 0x20) && (Rx_BUF[j] <= 0x7E)) {
                    unhook_ptr[unhook_len] = Rx_BUF[j];
                    ++unhook_len;
                    if (unhook_len >= z) break;
                }
            }
            if (unhook_len) unhook_save = false;
                           observe_GSM_STRING = true;
//             event::Loop::post(event::Event(events::GSM_HANDLED_STRING, unhook_len));
        }  */                  
//     }
    found_in = NULL;
    LOGGING({ cout << PGM << PSTR(" SCAN END\r\n"); });
    
}




} // !namespace gsm
