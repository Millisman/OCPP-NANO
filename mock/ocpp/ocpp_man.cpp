#include "ocpp_man.h"
#include "gsm/modem_manager.h"
#include "cjson/cJSON.h"
#include "utils/dump.h"
#include "store/strings.h"
#include "Base64.h"
#include "sha1.h"
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
gs_man(gsm::ManGSM::init()),
m_nov(0), m_prev(0),
m_nov_UT(0), m_prev_UT(0)
{
    message_wait = false;
    moment = 0;
    stack_push(Http_Connection_Upgrade);    
    gs_man.set_RecvHook(Recv);
    std::cout.setf(std::ios::unitbuf);
}

OCPP_States ManOCPP::stack_pop() {
    if (stack_count > 0) { return contents[--stack_count]; }
    else return NONE_STATE;
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

uint8_t ZZZ = 6;
const char str_BootNotification[] PROGMEM = "BootNotification";
const char jsn_BootNotification[] PROGMEM = "{\"chargePointVendor\":\"QQQQ\",\"chargePointModel\":\"WWWW\",\"chargePointSerialNumber\":\"1111\",\"chargeBoxSerialNumber\":\"2222\",\"firmwareVersion\":\"001\",\"iccid\":\"444\",\"imsi\":\"555\",\"meterSerialNumber\":\"77777\",\"meterType\":\"ADE77\"}";
// 
const char ocpp_message_call_code[] PROGMEM = "2";          //This message code is used when any RPC call is to be made
const char ocpp_message_response_code[] PROGMEM = "3";      //This message code is used when response of any RPC call is to be given
const char ocpp_message_error_code[] PROGMEM = "4"; 


void ManOCPP::write_Starting(OCPP_States state) { // setting to_server
    ocpp_current_send = state; //stack_pop();
    
    to_server = 0;

    switch (ocpp_current_send) {
        case Http_Connection_Upgrade:
            LOGGING( cout << PGM << PSTR("dddddd=") << ocpp_current_send << EOL; );
            
            for (uint8_t i = 0; i < 16; ++i) { ws_key.KeyStart[i] = random() % 256; }
            for (uint8_t i = 0; i < sizeof(ws_key.Key); ++i) { ws_key.Key[i] = '-'; }
            ws_key.Key[sizeof(ws_key.Key) - 1] = '\0';
            ocpp::base64_encode(ws_key.Key, ws_key.KeyStart, 16);
            to_server =  strlen_P(WSH_GET) + strlen_P(WSC_path) + strlen_P(WSH_HTTP) +
            strlen_P(WSC_protocol) + strlen_P(WSH_HOST) + strlen_P(WSC_host) + strlen_P(WSH_KEY) +
            strlen(ws_key.Key) + strlen_P(WSH_EOL2);
            break;
            
        case Heartbeat:
        {
            //to_server = sizeof(body_hb);
            MaskingStream ms(true, gs_man,  36 + ZZZ + sizeof(str_BootNotification) + sizeof(jsn_BootNotification) + sizeof(ocpp_message_call_code)
            );
            to_server = ms.get_hSize(true);

            
        }
        break;
        
        default: {
            LOGGING( cout << PGM << PSTR("Unhandled ocpp_current_send!") << ocpp_current_send << EOL; );
        }
    }
    
    if (to_server > 0) {
        LOGGING( cout << PGM << PSTR("Calling send_Schedule(to_server=") << to_server << EOL; );
        gs_man.send_Schedule(to_server);
    } else {
        mConnState = Conn_IDLE;
        LOGGING( cout << PGM << PSTR("to_server==0, skip task") << ocpp_current_send << EOL; );
    }

} 



void ManOCPP::writeCompletion() {
    LOGGING( cout << PGM << PSTR("writeCompletion") << ocpp_current_send << EOL; );
    switch (ocpp_current_send) {
        case Http_Connection_Upgrade:
            gs_man
            << PGM << WSH_GET   << PGM << WSC_path << PGM << WSH_HTTP  << PGM << WSC_protocol
            << PGM << WSH_HOST  << PGM << WSC_host << PGM << WSH_KEY << ws_key.Key << PGM << WSH_EOL2;
            break;

        case Heartbeat: {
            MaskingStream ms(true, gs_man, 36 + ZZZ + sizeof(str_BootNotification) + sizeof(jsn_BootNotification) + sizeof(ocpp_message_call_code) );
            Send_FrameHead_to_CMS(ms, Send_Frame_Code,      ocpp_message_call_code);
            Send_FrameHead_to_CMS(ms, Send_Frame_Action,    str_BootNotification);
            Send_FrameHead_to_CMS(ms, Send_Frame_Content,   jsn_BootNotification);
            Send_FrameHead_to_CMS(ms, Send_Frame_SEND, NULL);            
        }
        break;
            
        default: {
        }
    }
    to_server = 0;
}

bool ManOCPP::onread(uint8_t *payload, uint16_t length) {    
    std::cout << "\n onread(), ocpp_current_send=" << ocpp_current_send << EOL;

    switch (ocpp_current_send) {
        
        case Http_Connection_Upgrade: {
            std::cout << "\n onread(), ocpp_current_send= Http_Connection_Upgrade\n";
            if (websocket_check_upgrade(payload, length, &ws_key)) {
                std::cout << "\n onread(), websocket_check_upgrade= TRUE --> stack_push(Heartbeat);\n";
                stack_push(Heartbeat);
            };
        }
        break;
        
        case Heartbeat: {
            websocket_Recv((uint8_t *)payload, length);
        }
        break;
        
        default: {
            std::cout << "\n onread(), ocpp_current_send UNHANDLED!!!!!\n";
        }
        }
    return false;
}

// const char *test_buf = {"[3,\"1\",{\"status\":\"Accepted\",\"currentTime\":\"2022-02-11T21:15:25.350Z\",\"interval\":14400}]\r\n\0"};
// const char *test_buf = {"{\"status\":\"Accepted\",\"currentTime\":\"2022-02-11T21:15:25.350Z\",\"interval\":14400}"};

void ManOCPP::sssssparseOCPPFrame(uint8_t *payload, uint16_t length) {
    LOGGING(
        cout << PGM << PSTR("\r\n--Parse OCPP Frame size: ") << length << EOL;
        cout << (char *)payload;
        cout << PGM << PSTR("\r\n---END---\r\n");
    );    
    message_wait = false;
    message_Code_ptr = NULL;
    message_Code_len = 0;
    message_uuid_ptr = NULL;
    message_uuid_len = 0;
    message_jsonStr_ptr = NULL;
    message_jsonStr_len = 0;
    int16_t ps = strlen((char *)payload); // assigning delimiter to seperate the values
    if (ps > 16 ) {
        char *ptr = strtok((char *)(payload + 1), ","); // strtok function used from string.h library for segregating values
        if ((ptr != NULL)) {
            uint8_t ss = strlen(ptr);
            message_Code_ptr = ptr;
            message_Code_len = ss;
            ptr = strtok(NULL, ",");
            if ((ptr != NULL)) {
                message_uuid_ptr = ptr+1;
                message_uuid_len = strlen(ptr) - 2;
                ptr = strtok(NULL, ",");
                if ((ptr != NULL)) {
                    message_jsonStr_ptr = ptr;
                    message_jsonStr_len = ps - message_uuid_len -  message_Code_len - 8;  //strlen(ptr);
                    
                    if ((*message_jsonStr_ptr == '{') && 
                        (message_jsonStr_ptr[message_jsonStr_len - 1] == '}')) {
                        for (uint16_t x = 0; x < message_jsonStr_len; ++x) {
                            if (message_jsonStr_ptr[x] == 0) { message_jsonStr_ptr[x] = ','; } // TODO fix it
                        }
                        message_Code_ptr[message_Code_len] = 0;
                        message_uuid_ptr[message_uuid_len] = 0;
                        message_uuid_crc = utils::gencrc8((uint8_t *)message_uuid_ptr, message_uuid_len);
                        message_jsonStr_ptr[message_jsonStr_len] = 0;
                        message_wait = true;
//                         return true;
                        }
                }
            }
        }
    }
//     return false;
}



void ManOCPP::onEvent(const event::Event& event) {
    
    m_nov = mcu::Timer::millis();
    
    switch ((events)event.id) {
        
        case events::UPDATE:
            
            std::cout << '.';
            
            // ======================================
            // ======== message wait
            // ======================================
            if (message_wait) { // ALERT 
                message_wait = false;
                // process
                // 
                LOGGING (
                    cout << "CRC=" << message_uuid_crc << EOL;
                    cout << message_jsonStr_ptr << EOL;
                );
                
                
                message_cjson = cJSON_Parse(message_jsonStr_ptr);
                if (message_cjson == NULL) { // bad message
                } else {
                    cout << cJSON_GetObjectItem(message_cjson, "currentTime")->valuestring << EOL;
                }
                // ======================================
                // ======== or modem service
                // ======================================
            } else {
                if ((m_nov - m_prev) > 100) {
                    m_prev = m_nov;
                    
                    if (gs_man.is_Connected() && !gs_man.is_Busy()) { // ready to work
//                         if (stack_count > 0) {
                            switch (mConnState) {
                                case Conn_IDLE:
//                                     std::cout << "\n Conn_IDLE --> Conn_SEND_SIZE\n";
                                    mConnState = Conn_SEND_SIZE;
//                                     if (stack_count > 0) { mConnState = Conn_SEND_SIZE; }
                                    break;
                                case Conn_SEND_SIZE: {
                                    
                                    OCPP_States ocpp_tmp  = stack_pop();
                                    if (ocpp_tmp != NONE_STATE) {
                                        std::cout << "\n Found state = " << ocpp_tmp << '\n';
                                        write_Starting(ocpp_tmp);
                                        mConnState = Conn_SEND_DATA;
                                        std::cout << "\n Conn_SEND_SIZE --> Conn_SEND_DATA\n";
                                    } else {
//                                         std::cout << "\n Conn_SEND_SIZE --> Conn_IDLE [empty stack!]\n";
                                        mConnState = Conn_IDLE;
                                    }
                                }
                                break;
                                case Conn_SEND_DATA:
                                    
                                    writeCompletion();
                                    std::cout << "\n Conn_SEND_DATA --> Conn_IDLE\n";
                                    mConnState = Conn_IDLE;
                                    break;
                            }
                    } else { std::cout << "\nTransmitter not ready!\n"; }
                    }
                }
            break; // events::UPDATE
            
            default: {
                
            }
    }
}

void ManOCPP::Send_FrameHead_to_CMS(MaskingStream &ms, const Send_Frame_Enum level, const char* data) {
    switch (level) {        
        case Send_Frame_Code: {
            ms << '[' << data << ",\"";
            uint16_t num = 1;
            uuid_bytes_t binuuid;
            __uuid_generate_random(binuuid, &num);//calling function to generate GUID
            struct uuid_RFC4122 suuid;
            uuid_unpack(binuuid, &suuid);
            // UUID of 36 length hence allocation 37 for 36 + '\0' 
            // "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"
            ms << stream::HEX << stream::PAD_ZERO << suuid.time_low << '-';
            ms << stream::HEX << stream::PAD_ZERO << suuid.time_mid << '-';
            ms << stream::HEX << stream::PAD_ZERO << suuid.time_hi_and_version << '-';
            ms << stream::HEX << stream::PAD_ZERO << uint8_t(suuid.clock_seq >> 8),
            ms << stream::HEX << stream::PAD_ZERO << uint8_t(suuid.clock_seq & 0xFF) << '-';
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[0];
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[1];
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[2];
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[3];
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[4];
            ms << stream::HEX << stream::PAD_ZERO << suuid.node[5] << "\",\"";
        }
        break;
        
        case Send_Frame_Action:
            ms << data << "\",";
            break;
            
        case Send_Frame_Content:
            ms << data;
            break;
            
        case Send_Frame_SEND:
            ms << ']';
            break;
    }
}

}



// Sending BootNotification Packet
// [2,"10f61974-f500-4be4-a56e-6d4998013fb0","BootNotification",{"chargePointVendor":"A","chargePointModel":"Y"}]

// +IPD 70
// <0x81>:[2,"4b420865-d732-460d-b885-08da17e28ae3","ClearCache",{}]

/*
 * GSMM: ASCAN
 * HANDLE [+IPD] POS:115
 * +IPD 115
 * 224 onread onread115 content:<0x81>f[2,"455612d1-9ec0-438f-963a-2f02f79eab37","ChangeAvailability",{"connectorId":0,"type":"Inoperative"}]
 * <0x81>f[2,"455612d1-9ec0-438f-963a-2f02f79eab37","ChangeAvailability",{"connectorId":0,"type":"Inoperative"}]
 * +IPD P_ERR:0
 * GSMM: ASCAN
 * HANDLE [+IPD] POS:113
 * +IPD 113
 * 224 onread onread113 content:<0x81>d[2,"79a15712-fc9e-4dc6-a00a-e6752d1743d4","ChangeAvailability",{"connectorId":0,"type":"Operative"}]
 * <0x81>d[2,"79a15712-fc9e-4dc6-a00a-e6752d1743d4","ChangeAvailability",{"connectorId":0,"type":"Operative"}]
 * +IPD P_ERR:0
 * GSMM: ASCAN
 * HANDLE [+IPD] POS:115
 * +IPD 115
 * 224 onread onread115 content:<0x81>f[2,"330ccd88-b537-436e-821b-9b8a6e1cdc8b","ChangeAvailability",{"connectorId":1,"type":"Inoperative"}]
 * <0x81>f[2,"330ccd88-b537-436e-821b-9b8a6e1cdc8b","ChangeAvailability",{"connectorId":1,"type":"Inoperative"}]
 * +IPD P_ERR:0
 * GSMM: ASCAN
 * HANDLE [+IPD] POS:113
 * +IPD 113
 * 224 onread onread113 content:<0x81>d[2,"dfeb622d-58e7-45f2-8b9d-0b289e2852a1","ChangeAvailability",{"connectorId":2,"type":"Operative"}]
 * <0x81>d[2,"dfeb622d-58e7-45f2-8b9d-0b289e2852a1","ChangeAvailability",{"connectorId":2,"type":"Operative"}]
 * +IPD P_ERR:0*/



// 224 onread onread107 content:<0x81>_[2,"8ab0b16f-a37c-459a-852b-75e546ef754f","ChangeConfiguration",{"key":"sernum","value":"777"}]
// <0x81>_[2,"8ab0b16f-a37c-459a-852b-75e546ef754f","ChangeConfiguration",{"key":"sernum","value":"777"}]
// +IPD P_ERR:0

// +IPD 112
// 224 onread onread112 content:<0x81>c[2,"5dbe1122-17e2-4576-9882-9fbd47ee852d","ChangeConfiguration",{"key":"BlinkRepeat","value":"44"}]
// <0x81>c[2,"5dbe1122-17e2-4576-9882-9fbd47ee852d","ChangeConfiguration",{"key":"BlinkRepeat","value":"44"}]
// +IPD P_ERR:0


// 224 onread onread107 content:<0x81>_[2,"8ab0b16f-a37c-459a-852b-75e546ef754f","ChangeConfiguration",{"key":"sernum","value":"777"}]
// <0x81>_[2,"8ab0b16f-a37c-459a-852b-75e546ef754f","ChangeConfiguration",{"key":"sernum","value":"777"}]
// +IPD P_ERR:0
// GSMM: ASCAN
// HANDLE [+IPD] POS:112
// +IPD 112
// 224 onread onread112 content:<0x81>c[2,"5dbe1122-17e2-4576-9882-9fbd47ee852d","ChangeConfiguration",{"key":"BlinkRepeat","value":"44"}]
// <0x81>c[2,"5dbe1122-17e2-4576-9882-9fbd47ee852d","ChangeConfiguration",{"key":"BlinkRepeat","value":"44"}]
// +IPD P_ERR:0
// GSMM: ASCAN
// HANDLE [+IPD] POS:84
// +IPD 84
// 224 onread onread84 content:<0x81>H[2,"1174999e-a7b6-4a1e-a6d3-ce5bf58b1fac","GetConfiguration",{"key":[]}]
// <0x81>H[2,"1174999e-a7b6-4a1e-a6d3-ce5bf58b1fac","GetConfiguration",{"key":[]}]
// +IPD P_ERR:0


