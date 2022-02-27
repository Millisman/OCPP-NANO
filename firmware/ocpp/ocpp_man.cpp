#include "ocpp_man.h"
#include "store/strings.h"
#include "Base64.h"
#include "uuid.h"
#define  LOG_ON

#ifdef LOG_ON
#define LOGGING(x) cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif

namespace ocpp {

using stream::PGM;

ManOCPP::ManOCPP():
    cout(mcu::Serial0::init()),
    gs_man(gsm::ManGSM::get()),
    ms(true, gs_man),
    ocpp_parser(ms),
    m_nov(0), m_prev(0),
    m_nov_UT(0), m_prev_UT(0)
{
    message_wait = false;
    moment = 0;
    mConnState = Conn_IDLE;
    stack_push(Http_Connection_Upgrade);    
    gs_man.set_RecvHook(Recv);
#ifndef __AVR__
    std::cout.setf(std::ios::unitbuf);
#endif
}

void ManOCPP::write_Starting(OCPP_States state) {
    
//     memset(rx_buf, 0, rx_sz);
//     memset(tx_buf, 0, tx_sz);
    
    LOGGING( cout << PGM << PSTR("write_Starting:") << state << EOL; );
    OCPP_Current_State_Req = state; //stack_pop();
    to_server = 0;
    if (OCPP_Current_State_Req == Http_Connection_Upgrade) {
            for (uint8_t i = 0; i < 16; ++i) { ws_key.KeyStart[i] = random() % 256; }
            for (uint8_t i = 0; i < sizeof(ws_key.Key); ++i) { ws_key.Key[i] = '-'; }
            ws_key.Key[sizeof(ws_key.Key) - 1] = '\0';
            ocpp::base64_encode(ws_key.Key, ws_key.KeyStart, 16);
            to_server =  strlen_P(WSH_GET) + strlen_P(WSC_path) + strlen_P(WSH_HTTP) + strlen_P(WSC_host) + strlen_P(WSH_KEY) +
            strlen(ws_key.Key) + 4; // TODO grab from EEPROM
    } else {
        ms.set_Size(ocpp_parser.write_OCPP_state(OCPP_Current_State_Req, false));
        to_server = ms.get_hSize(true);
    }
    if (to_server > 0) {
        LOGGING( cout << PGM << PSTR("to_server=") << to_server << EOL; );
        gs_man.send_Schedule(to_server);
    } else { mConnState = Conn_IDLE; }
}

void ManOCPP::writeCompletion() {
    LOGGING( cout << PGM << PSTR("writeCompletion") << EOL; );
    if (OCPP_Current_State_Req == Http_Connection_Upgrade) {
        LOGGING( cout << PGM << PSTR("gs_man write data") << EOL; );
        gs_man << PGM << WSH_GET << PGM << WSC_path << PGM << WSH_HTTP << PGM << WSC_host << PGM << WSH_KEY << ws_key.Key << EOL << EOL;
        //cout << PGM << WSH_GET << PGM << WSC_path << PGM << WSH_HTTP << PGM << WSC_host << PGM << WSH_KEY << ws_key.Key << EOL << EOL;
        
        LOGGING( cout << PGM << PSTR("gs_man write data end") << EOL; );
    } else {
        LOGGING( cout << PGM << PSTR("write_OCPP_state") << EOL; );
        ocpp_parser.write_OCPP_state(OCPP_Current_State_Req, true);
    }
    
    cout << EOL;
    for (uint16_t zz = 0; zz < to_server; ++zz) { cout << tx_buf[zz];  }
    cout << EOL;
    for (uint16_t zz = 0; zz < to_server; ++zz) { cout << stream::PAD_ZERO << stream::HEX << tx_buf[zz];  }
    cout << EOL;
    
    
    to_server = 0;
}

#define WS_CHECKTABLE_COUNT 3
const char *ws_checktable[WS_CHECKTABLE_COUNT] = {
    str_upgrade, str_websocket, str_Connection //, check3
};

bool ManOCPP::onread(uint8_t *payload, uint16_t length) {
    if (length > 512) return false; // TODO check length
    if (OCPP_Current_State_Req == Http_Connection_Upgrade) {
        if ((strstr_P((const char *)payload, str_http_11) != NULL) ||
            (strstr_P((const char *)payload, str_http_10) != NULL)) {
                uint8_t check = 0;
                for (uint8_t x = 0; x < WS_CHECKTABLE_COUNT; ++x) {
                    if (strcasestr_P((const char *)payload, ws_checktable[x]) != NULL) { check++; }
                    if (check == WS_CHECKTABLE_COUNT) {
                        if (strcasestr_P((const char *)payload, str_SecWsAccept) != NULL) {
                            Sha1.set_m((sha1_mem *)tx_buf);
                            Sha1.init();
                            for (uint8_t a = 0; a < strlen(ws_key.Key); ++a) { Sha1.write(ws_key.Key[a]); }
                            char m; char *mp = (char *)str_UUID;
                            while ((m = pgm_read_byte(mp++))) { Sha1.write(m); }
                            uint8_t *hash = Sha1.result();
                            ws_key.KeyStart[base64_encode(ws_key.KeyStart, (char *)hash, 20)] = '\0';
                            if (strcasestr((const char *)payload, ws_key.KeyStart) != NULL) {
                                stack_push(BootNotification);
                                online = true; // TODO check accept result, check timeout
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
    } else {
        if (length < 2) { return false; }
        //uint8_t fin =   (payload[0] & 0x80) == 0x80 ? 1 : 0; // msg_fin = (in_buffer[0] >> 7) & 0x01;
        //uint8_t opcode = payload[0] & 0x0F; 
        if ((payload[1] & 0x80) != 0x0) { return false; } // WS_COMPLETE_FRAME_MASKED_ERR
        uint8_t msg_masked = (payload[1] >> 7) & 0x01;
        uint8_t payload_offset = 2;
        uint8_t length_field = payload[1] & (~0x80);
        uint16_t payload_length = 0;
        if(length_field <= 125) {
            payload_length = length_field;
        } else if(length_field == 126) { // msglen is 16bit!
            payload_length = ( (payload[2] << 8) | (payload[3]) );
            payload_offset += 2;
        } else { return false; };
        if (length < payload_length + payload_offset) { return false; } // INCOMPLETE_FRAME
        if (msg_masked) {
            uint32_t mask = *((uint32_t *)(payload + payload_offset));
            payload_offset += 4;
            uint8_t *c = payload + payload_offset;
            for(uint16_t i=0; i < payload_length; i++) { // unmask payload:
                c[i] = c[i] ^ ((uint8_t *)(&mask))[i % 4];
            }
        }
        
        message_wait = ocpp_parser.check_OCPP_Frame(payload + payload_offset, length - payload_offset);
    }
    return true;
    

    
}


void ManOCPP::onEvent(const event::Event& event) {
    
    m_nov = mcu::Timer::millis();
    
    switch ((events)event.id) {
        
        case events::UPDATE:
            if (message_wait) {
                message_wait = false;
                ocpp_parser.processing(OCPP_Current_State_Req);
            } else {
                if ((m_nov - m_prev) > 100) {
                    m_prev = m_nov;
                    if (gs_man.is_Connected()) {
                        switch (mConnState) {
                            case Conn_IDLE:
                                mConnState = Conn_SEND_SIZE;
                                break;
                                
                            case Conn_SEND_SIZE: {
                                
                                if (!gs_man.is_Busy()) {
                                    OCPP_States ocpp_tmp  = stack_pop();
                                    if (ocpp_tmp != NONE_STATE) {
                                        write_Starting(ocpp_tmp);
                                        mConnState = Conn_SEND_DATA;
                                    } else { mConnState = Conn_IDLE; }
                                } else { mConnState = Conn_IDLE; }
                            }
                            break;
                            
                            case Conn_SEND_DATA:
                                
                                if (!gs_man.is_Busy()) {
                                    writeCompletion();
                                    mConnState = Conn_IDLE;
                                }
//                                 writeCompletion();
//                                 mConnState = Conn_IDLE;
                                break;
                                
                        }
                    }
                }
            }
            break;
        
    case events::TICK_1_SEC:
        if (online) {
            
            counter_Heartbeat--; // Charge points will send heartbeats roughly every 3 minutes
            if (counter_Heartbeat == 0) {
                counter_Heartbeat = 5; // TODO interval set
                stack_push(Heartbeat);
            }
            
            counter_Authorize--;
            if (counter_Authorize == 0) {
                counter_Authorize = 5; // TODO interval set
                stack_push(Authorize);
            }

            counter_s1--;
            if (counter_s1 == 0) {
                counter_s1 = 10; // TODO interval set
                stack_push(StartTransaction);
            }
            
            counter_s2--;
            if (counter_s2 == 0) {
                counter_s2 = 20; // TODO interval set
                stack_push(StopTransaction);
            }
            
//             counter_StatusNotification--;
//             if (counter_StatusNotification == 0) {
//                 counter_StatusNotification = 5; // TODO interval set
//                 stack_push(StatusNotification);
//             }
        }
        break;

    default: {}
    }
}


OCPP_States ManOCPP::stack_pop() {
    OCPP_States result = NONE_STATE;
//     if (gs_man.is_Connected() && !gs_man.is_Busy()) {
        if (stack_count > 0) { result = contents[--stack_count]; }
//     }
    return result;
}

void ManOCPP::stack_push(const OCPP_States c) {
    LOGGING( cout << PGM << PSTR("Stack push:") << c << EOL; );
    if (stack_count < STACK_SIZE) {
        for (uint8_t x = 0; x < stack_count; x++) {
            if (contents[x] == c) return; // prevent duplicates
        }
        contents[stack_count++] = c;
    }
}


}
