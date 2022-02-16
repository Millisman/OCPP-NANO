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
#include "cjson/cJSON.h"
#include "gsm/modem_manager.h"
#include "websocket.h"


const char WSH_GET[]  PROGMEM = "GET ";
const char WSH_HTTP[] PROGMEM = " HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Protocol: ";
const char WSH_HOST[] PROGMEM = "\r\nHost: ";
const char WSH_KEY[]  PROGMEM = "\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: ";
const char WSH_EOL2[] PROGMEM = "\r\n\r\n";
// + 4

const char WSC_host[]     PROGMEM = "evseman.solar-track.me";
const char WSC_port[]     PROGMEM = "80";
const char WSC_path[]     PROGMEM = "/ocpp/websocket/CentralSystemService/1";
const char WSC_protocol[] PROGMEM = "ocpp1.6";

#define STACK_SIZE 4

namespace ocpp {

typedef enum : uint8_t {
    NONE_STATE,
    Http_Connection_Upgrade,
    Authorize,
    BootNotification,
    CancelReservation,
    ChangeAvailability,
    ChangeConfiguration,
    ClearCache,
    ClearChargingProfile,
    DataTransfer,
    DiagnosticsStatusNotification,
    FirmwareStatusNotification,
    GetConfiguration,
    GetDiagnostics,
    GetLocalListVersion,
    Heartbeat,
    MeterValues,
    RemoteStartTransaction,
    RemoteStopTransaction,
    GetCompositeSchedule,
    ReserveNow,
    Reset,
    StartTransaction,
    StatusNotification,
    StopTransaction,
    SendLocalList,
    SetChargingProfile,
    TriggerMessage,
    UnlockConnector,
    UpdateFirmware
} OCPP_States;


enum Send_Frame_Enum {Send_Frame_Code, Send_Frame_Action, Send_Frame_Content, Send_Frame_SEND};

enum ConnStates {Conn_IDLE, Conn_SEND_SIZE, Conn_SEND_DATA};

class MaskingStream: public stream::OutputStream {
    uint8_t mH[4], mM[4];
    uint16_t count = 0;
    uint16_t hSz  = 0;
    uint16_t cSz = 0;
    bool msk = false;
    bool hpc = false;

public:
    explicit MaskingStream(bool _msk, stream::OutputStream &sos, uint16_t in_sz):
    msk(_msk), os(sos), cSz(in_sz)
    { }
        
    MaskingStream &operator>>(char &ch);
    
    uint16_t get_hSize(bool Full = false) {
        if(cSz < 126) { hSz = 2;
        } else { hSz = 4; }
        if (msk) { hSz += sizeof(mM); }
        if (Full) hSz += cSz;
        return hSz;
    }
    
private:
    
    void write(const char ch) override {
        if (!hpc) {
            hpc = true;
            for (uint8_t x = 0; x < sizeof(mH); ++x) { mH[x] = 0; }
            mH[0] = 0x81; // fin + opcode txt
            if (msk) { mH[1] |= bit(7); }
            if(cSz < 126) {
                mH[1] |= cSz;
            } else {
                mH[1] |= 126;
                mH[2] = ((cSz >> 8) & 0xFF);
                mH[3] = (cSz & 0xFF);
            }
            get_hSize();
            for (uint8_t x = 0; (x < sizeof(mH) && (x < hSz)); ++x) {
                os << (char)(mH[x]);
            }
            if (msk) {
                for (uint8_t x = 0; x < sizeof(mM); ++x) {
                    mM[x] = rand() % 256;
                    os << (char)mM[x];
                }
            }
        }
        if (msk) { os << (char)(ch ^ mM[count % 4]); } else
        { os << ch; }
        count++;
    }
    stream::OutputStream &os;
};

    
class ManOCPP : public event::Handler {
    stream::UartStream cout;
    gsm::ManGSM &gs_man;
    uint32_t m_nov, m_prev;
    uint32_t m_nov_UT, m_prev_UT, moment;
    ws_key_struct ws_key;
    ConnStates mConnState = Conn_IDLE;
    char    *message_Code_ptr;
    uint8_t  message_Code_len;
    char    *message_uuid_ptr;
    uint8_t  message_uuid_len;
    uint8_t  message_uuid_crc;    
    char    *message_jsonStr_ptr;
    uint8_t  message_jsonStr_len;
    cJSON   *message_cjson;
    bool     message_wait;
public:
    bool onread(uint8_t *payload, uint16_t length);
    void  update();
    static ManOCPP& init() {
        static ManOCPP man_ocpp;
        return man_ocpp;
    }
    
    static void Recv(uint8_t *payload, uint16_t length) {
        if (payload == NULL) {
            std::cout << "\r\n--Recv NULL payload: " << length << EOL;
            return;
        }
        ManOCPP::init().onread(payload, length);
    }
    
    
private:
    ManOCPP();
    void onEvent(const event::Event& event) override;
    DISALLOW_COPY_AND_ASSIGN(ManOCPP);
    void sssssparseOCPPFrame(uint8_t *payload, uint16_t length);
    void Send_FrameHead_to_CMS(MaskingStream &ms, const Send_Frame_Enum level, const char* data);
    uint8_t stack_count;
    OCPP_States contents[STACK_SIZE];    
    OCPP_States stack_pop();
    void stack_push(const OCPP_States c);
    void write_Starting(OCPP_States state);
    void writeCompletion();
    OCPP_States ocpp_current_send;
    uint16_t to_server;
};

}  // namespace ocpp

