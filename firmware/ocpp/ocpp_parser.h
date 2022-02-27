#pragma once
#include "mcu/mcu.h"
#include "events.h"
#include "event/loop.h"
#include "event/handler.h"
#include "utils/cpp.h"
#include "mcu/timer.h"
#include "store/settings.h"

#include "ocpp_strings.h"
#include "uuid.h"
#include "utils/rtc_lib.h"
#include "mjson.h"

#define  LOG_ON

#ifdef LOG_ON
#define LOGGING(x) cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif



namespace ocpp {
    
using stream::PGM;
// #define in_range(c, lo, up)  ((uint8_t)c >= lo && (uint8_t)c <= up)
// #define isdigit(c)           in_range(c, '0', '9')
    
typedef struct __attribute__((__packed__)) {
    const char *key;
    const char *value;
} cons_string_pair;

enum Send_Frame_Enum {Send_Frame_Code, Send_Frame_Action, Send_Frame_Content, Send_Frame_SEND};

const char pval_chargePointVendor[] PROGMEM = "Millisman";
const char pval_chargePointModel[]  PROGMEM = "TestStation";
const char pval_chargePointSerialNumber[]  PROGMEM = "1000";
const char pval_firmwareVersion[] PROGMEM = "apha 0.1.1";
//---------
const char pval_chargeBoxSerialNumber[] PROGMEM = "777";
const char pval_iccid[] PROGMEM = ">iccid<";
const char pval_imsi[] PROGMEM = ">imsi<";
const char pval_meterSerialNumber[] PROGMEM = "001";
const char pval_meterType[] PROGMEM = "SPI_ADE7758";

const struct {
    const char   *val_chargePointVendor  = pval_chargePointVendor;
    const char   *val_chargePointModel   = pval_chargePointModel;
    const char   *val_chargePointSerialNumber   = pval_chargePointSerialNumber;
    const char   *val_firmwareVersion  = pval_firmwareVersion;
    //---------
    const char   *val_chargeBoxSerialNumber  = pval_chargeBoxSerialNumber;
    const char   *val_iccid  = pval_iccid;
    const char   *val_imsi  = pval_imsi;
    const char   *val_meterSerialNumber  = pval_meterSerialNumber;
    const char   *val_meterType  = pval_meterType;
} ocpp_def_str;
    

// class StreamCounter: public stream::OutputStream {
//     uint16_t cnt;
// public:
//     explicit StreamCounter(): cnt(0) { }
//     uint16_t get_Size() { return cnt; }
// private:
//     void write(const char ch) override { cnt++; }
// };

    
class MaskingStream: public stream::OutputStream {
    uint8_t mH[4], mM[4]; //, count_h;
    uint16_t count;
    uint16_t cSz;
    bool msk;
    bool hpc;
    bool enabled;
    
public:
    explicit MaskingStream(bool _msk, stream::OutputStream &sos):
    msk(_msk), os(sos), enabled(false) { 
        set_Size(0);
        for (uint8_t x = 0; x < sizeof(mM); ++x) {
            mM[x] = rand() % 256; }
    }
    
    MaskingStream &operator>>(char &ch);
    
    void set_Size(uint16_t in_sz) {
        cSz = in_sz;
        hpc = false;
        count = 0;
    }
    
    uint16_t get_hSize(bool Full = false) {
        uint16_t hSz = 0;
        if(cSz < 126) { hSz = 2;
        } else { hSz = 4; }
        if (msk) { hSz += sizeof(mM); }
        if (Full) hSz += cSz;
        return hSz;
    }
    
    uint16_t get_Printed() { return count; }
    
    
    void Enable(const bool en) {
        hpc = !en;
        enabled = en;
        count = 0;
    }
    
private:
    
    void write(const char ch) override {
        if (!hpc) {
            hpc = true;
            count = 0;
            uint8_t hSz = 0;
            for (uint8_t x = 0; x < sizeof(mH); ++x) { mH[x] = 0; }
            mH[0] = 0x81; // fin + opcode txt
            if (msk) { mH[1] |= bit(7); }
            if(cSz < 126) {
                mH[1] |= cSz;
                hSz =2;
            } else {
                hSz = 4;
                mH[1] |= 126;
                mH[2] = ((cSz >> 8) & 0xFF);
                mH[3] = (cSz & 0xFF);
            }
            for (uint8_t x = 0; x < hSz; ++x) { os << (char)(mH[x]); }
            if (msk) {
                for (uint8_t x = 0; x < sizeof(mM); ++x) {
                    if (enabled) { os << (char)mM[x]; }
                }
            }
        }
        
        if (enabled) {
            
            if (msk) {
                os << (char)(ch ^ mM[count % 4]);
            } else {
                os << ch;
            }
        }
        count++;
    }
    stream::OutputStream &os;
};
    
    
    

typedef enum : uint8_t {
    NONE_STATE,             Http_Connection_Upgrade, 
    
    Authorize,              BootNotification,               Heartbeat,
    MeterValues,            SetChargingProfile,             StatusNotification,
    StartTransaction,       StopTransaction,                TriggerMessage,
    RemoteStartTransaction, RemoteStopTransaction,          ChangeConfiguration,
    GetConfiguration,       Reset, UpdateFirmware,          FirmwareStatusNotification,
    GetDiagnostics,         DiagnosticsStatusNotification,  UnlockConnector,
    ClearChargingProfile,   ChangeAvailability,
    // ----------------
    CancelReservation,      ClearCache,             DataTransfer,
    GetLocalListVersion,    GetCompositeSchedule,   ReserveNow,
    SendLocalList,
    // ----------------
    OCPP_STATE_MIN = Authorize, OCPP_STATE_MAX = SendLocalList
} OCPP_States;

typedef struct __attribute__((__packed__)) {
    const OCPP_States state;
    const char *label;
} OCPP_State_pair;

#define OCPP_STATES_COUNT 28

const OCPP_State_pair OCPP_sp[OCPP_STATES_COUNT] = {
    { Authorize,            str_Authorize },
    { BootNotification,     str_BootNotification },
    { Heartbeat,            str_Heartbeat },
    { MeterValues,          str_MeterValues },
    { SetChargingProfile,   str_SetChargingProfile },
    { StatusNotification,   str_StatusNotification },
    { StartTransaction,     str_StartTransaction },
    { StopTransaction,      str_StopTransaction },
    { TriggerMessage,       str_TriggerMessage },
    { RemoteStartTransaction,           str_RemoteStartTransaction },
    { RemoteStopTransaction,            str_RemoteStopTransaction },
    { ChangeConfiguration,              str_ChangeConfiguration },
    { GetConfiguration,                 str_GetConfiguration },
    { Reset,                            str_Reset },
    { UpdateFirmware,                   str_UpdateFirmware },
    { FirmwareStatusNotification,       str_FirmwareStatusNotification },
    { GetDiagnostics,                   str_GetDiagnostics },
    { DiagnosticsStatusNotification,    str_DiagnosticsStatusNotification },
    { UnlockConnector,      str_UnlockConnector },
    { ClearChargingProfile, str_ClearChargingProfile },
    { ChangeAvailability,   str_ChangeAvailability },
    // -------------------------------------------
    { CancelReservation,    str_CancelReservation },
    { ClearCache,           str_ClearCache },
    { DataTransfer,         str_DataTransfer },
    { GetLocalListVersion,  str_GetLocalListVersion },
    { GetCompositeSchedule, str_GetCompositeSchedule },
    { ReserveNow,           str_ReserveNow },
    { SendLocalList,        str_SendLocalList }
};



typedef enum: uint8_t {
    RegistrationStatus_UNKNOWN,
    RegistrationStatus_Accepted,
    RegistrationStatus_Pending,
    RegistrationStatus_Rejected
} OCPP_RegistrationStatus;

typedef struct __attribute__((__packed__)) {
    const OCPP_RegistrationStatus state;
    const char *label;
} OCPP_RegistrationStatus_pair;

#define REGISTRATIONSTATUS_COUNT 3
const OCPP_RegistrationStatus_pair OCPP_RegStaPair[REGISTRATIONSTATUS_COUNT] = {
    { RegistrationStatus_Accepted, str_Accepted },
    { RegistrationStatus_Pending,  str_Pending },
    { RegistrationStatus_Rejected, str_Rejected }
};

typedef enum: uint8_t { 
    AuthorizationStatus_UNKNOWN,
    AuthorizationStatus_Accepted,
    AuthorizationStatus_Blocked,
    AuthorizationStatus_Expired,
    AuthorizationStatus_Invalid,
    AuthorizationStatus_ConcurrentTx
} OCPP_AuthorizationStatus;

typedef struct __attribute__((__packed__)) {
    const OCPP_AuthorizationStatus state;
    const char *label;
} OCPP_AuthorizationStatus_pair;

#define AUTHORIZATIONSTATUS_COUNT 5
const OCPP_AuthorizationStatus_pair OCPP_AuthStaPair[AUTHORIZATIONSTATUS_COUNT] = {
    { AuthorizationStatus_Accepted,     str_Accepted },
    { AuthorizationStatus_Blocked,      str_Blocked },
    { AuthorizationStatus_Expired,      str_Expired },
    { AuthorizationStatus_Invalid,      str_Invalid },
    { AuthorizationStatus_ConcurrentTx, str_ConcurrentTx }
};

#define in_range(c, lo, up)  ((uint8_t)c >= lo && (uint8_t)c <= up)
#define isdigit(c)           in_range(c, '0', '9')

class OCPP_Parser {
    stream::UartStream cout;
    MaskingStream &ms;
    // -------- UUID RFC4122 --------
    uint16_t        uuid_n = 1;
    uuid_bytes_t    uuid_b;
    uuid_RFC4122    uuid_s;
    // ------------------------------
    char    *message_Code_ptr;
    uint8_t  message_Code_len;
    char    *message_uuid_ptr;
    uint8_t  message_uuid_len;
    char    *message_request_ptr;
    uint8_t  message_request_len;
    char    *message_jsonStr_ptr;
    uint16_t message_jsonStr_len;
    
    
    char    *json_payload;
    uint16_t json_payload_length;
    
    cons_string_pair tmp_str_pair;
    
    char str_processing_uuid[37];
    
    OCPP_AuthorizationStatus    Authorize_AuthorizationStatus;
    char                        Authorize_expiryDate[28];
    
    uint32_t                BootNotification_interval;
    OCPP_RegistrationStatus BootNotification_RegistrationStatus; // TODO UNKNOWN -def
    
    int tmp_int0, tmp_int1;
    const char *tmp_char0, *tmp_char1;
    double m_BufDouble;

    
    uint32_t UnixTime;
    
    
    bool load_System_UnixTime(const char *str, const int len) {  // {"currentTime":"2022-02-19T03:16:16.633Z"}
    if ((mjson_find(str, len, "$.currentTime", &tmp_char0, &tmp_int0) == MJSON_TOK_STRING)) {
        if (tmp_int0 > 18) {
            if (isdigit(tmp_char0[1]) || isdigit(tmp_char0[2]) ||
                isdigit(tmp_char0[3]) || isdigit(tmp_char0[4]) ||
                tmp_char0[5] == '-' ||
                isdigit(tmp_char0[6]) || isdigit(tmp_char0[7]) ||
                tmp_char0[8] == '-' ||
                isdigit(tmp_char0[9]) || isdigit(tmp_char0[10]) ||
                tmp_char0[11] == 'T' ||
                isdigit(tmp_char0[12]) || isdigit(tmp_char0[13]) ||
                tmp_char0[14] == ':' ||
                isdigit(tmp_char0[15]) || isdigit(tmp_char0[16]) ||
                tmp_char0[17] == ':' ||
                isdigit(tmp_char0[18]) || isdigit(tmp_char0[19])) {
                utils::DateTime dt(
                    (tmp_char0[1] - '0') * 1000 +
                    (tmp_char0[2] - '0') * 100 +
                    (tmp_char0[3] - '0') * 10 +
                    (tmp_char0[4] - '0'),
                    ( tmp_char0[6]-'0')*10 + ( tmp_char0[7]-'0'),
                    ( tmp_char0[9]-'0')*10 + ( tmp_char0[10]-'0'),
                    (tmp_char0[12]-'0')*10 + (tmp_char0[13]-'0'),
                    (tmp_char0[15]-'0')*10 + (tmp_char0[16]-'0'),
                    (tmp_char0[18]-'0')*10 + (tmp_char0[19]-'0'));
                UnixTime = dt.unixtime();
                LOGGING( cout << PGM << PSTR("adjust_rtc UT=") << UnixTime << EOL; );
                store::EepromSettings::init().set_UnixTime(UnixTime);
                return true;
                }
        }
        }
        return false;
    }
    
    void write_Time_to_ocpp() {
        utils::DateTime dt(store::EepromSettings::now()); 
        uint8_t year = dt.year() - 2000;
        ms << "\"20" 
        << (char)(((year / 10) % 10)  + '0')
        << (char)(((year / 1) % 10)  + '0') << '-'
        << (char)((((dt.month() + 1) / 10) % 10)  + '0')
        << (char)((((dt.month() + 1) / 1) % 10)  + '0') << '-'
        << (char)((((dt.day() + 1) / 10) % 10)  + '0')
        << (char)((((dt.day() + 1) / 1) % 10)  + '0') << 'T'
        << (char)(((dt.hour() / 10) % 10)  + '0')
        << (char)(((dt.hour() / 1) % 10)  + '0') << ':'
        << (char)(((dt.minute() / 10) % 10)  + '0')
        << (char)(((dt.minute() / 1) % 10)  + '0') << ':'
        << (char)(((dt.second() / 10) % 10)  + '0')
        << (char)(((dt.second() / 1) % 10)  + '0')
        << ".000Z\""; 
    }
    
    uint32_t TransactionId = 0;
    
public:
    OCPP_Parser(MaskingStream &_ms);

    // void processing_call(OCPP_States call_State, const char *uuid_str, const char *obj_str, const int obj_len); server side
    void processing_response(OCPP_States from_State, const char *uuid_str, const char *obj_str, const int obj_len);
    void processing_error(OCPP_States from_State, const char *uuid_str, const char *obj_str, const int obj_len);
    
    void processing(OCPP_States from_State);
    bool check_OCPP_Frame(uint8_t *payload, uint16_t length);
    uint16_t write_OCPP_state(OCPP_States state, bool print);
    void write_UUID(const bool regen); // 36 B
    void write_key_value(const cons_string_pair *sp, const bool inram = false);
    void write_key_value(const char *key, const int32_t value);
    void write_comma(const uint8_t current, const uint8_t target);
    void write_json_start_P(const char *str);
    
    // Gets called by the RPC engine to send a reply frame
    int m_Sender(const char *frame, int frame_len, void *privdata) {
        for (int a=0; a < frame_len; ++a) {
            ms << (char)frame[a];
            // printf("%c",frame[a] );
        }
        return frame_len;
    }
    
    void print_OCPP_States(OCPP_States state) {
#ifdef LOG_ON
        cout << PGM << PSTR("OCPP State: ") << state << ' ';
        switch (state) {
            case NONE_STATE:                    cout << "NONE"; break;
            case Http_Connection_Upgrade:       cout << PGM << str_upgrade; break;
            case Authorize:                     cout << PGM << str_Authorize; break;
            case BootNotification:              cout << PGM << str_BootNotification; break;
            case Heartbeat:                     cout << PGM << str_Heartbeat; break;
            case MeterValues:                   cout << PGM << str_MeterValues; break;
            case SetChargingProfile:            cout << PGM << str_SetChargingProfile; break;
            case StatusNotification:            cout << PGM << str_StatusNotification; break;
            case StartTransaction:              cout << PGM << str_StartTransaction; break;
            case StopTransaction:               cout << PGM << str_StopTransaction; break;
            case TriggerMessage:                cout << PGM << str_TriggerMessage; break;
            case RemoteStartTransaction:        cout << PGM << str_RemoteStartTransaction; break;
            case RemoteStopTransaction:         cout << PGM << str_RemoteStopTransaction; break;
            case ChangeConfiguration:           cout << PGM << str_ChangeConfiguration; break;
            case GetConfiguration:              cout << PGM << str_GetConfiguration; break;
            case Reset:                         cout << PGM << str_Reset; break;
            case UpdateFirmware:                cout << PGM << str_UpdateFirmware; break;
            case FirmwareStatusNotification:    cout << PGM << str_FirmwareStatusNotification; break;
            case GetDiagnostics:                cout << PGM << str_GetDiagnostics; break;
            case DiagnosticsStatusNotification: cout << PGM << str_DiagnosticsStatusNotification; break;
            case UnlockConnector:               cout << PGM << str_UnlockConnector; break;
            case ClearChargingProfile:          cout << PGM << str_ClearChargingProfile; break;
            case ChangeAvailability:            cout << PGM << str_ChangeAvailability; break;
            // ----------------
            case CancelReservation:             cout << PGM << str_CancelReservation; break;
            case ClearCache:                    cout << PGM << str_ClearCache; break;
            case DataTransfer:                  cout << PGM << str_DataTransfer; break;
            case GetLocalListVersion:           cout << PGM << str_GetLocalListVersion; break;
            case GetCompositeSchedule:          cout << PGM << str_GetCompositeSchedule; break;
            case ReserveNow:                    cout << PGM << str_ReserveNow; break;
            case SendLocalList:                 cout << PGM << str_SendLocalList; break;
            // ----------------
        }
        cout << EOL;
#endif
    }
    
};






}
