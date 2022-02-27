#include "ocpp_parser.h"
#include "ocpp_strings.h"
#include "store/strings.h"
#include "Base64.h"
#include "sha1.h"
#include "uuid.h"
#include "store/settings.h"
#include "utils/rtc_lib.h"
namespace ocpp {

using stream::PGM;

OCPP_Parser::OCPP_Parser(MaskingStream &_ms):
    cout(mcu::Serial0::init())
    , ms(_ms)
{ }




void OCPP_Parser::write_UUID(const bool regen) {
    if (regen) {
        __uuid_generate_random(uuid_b, &uuid_n); //calling function to generate GUID
        uuid_unpack(uuid_b, &uuid_s);
        // UUID = 36 B
    }
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.time_low << '-';
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.time_mid << '-';
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.time_hi_and_version << '-';
    ms << stream::HEX << stream::PAD_ZERO << uint8_t(uuid_s.clock_seq >> 8),
    ms << stream::HEX << stream::PAD_ZERO << uint8_t(uuid_s.clock_seq & 0xFF) << '-';
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[0];
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[1];
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[2];
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[3];
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[4];
    ms << stream::HEX << stream::PAD_ZERO << uuid_s.node[5];
}


void OCPP_Parser::processing(OCPP_States from_State) {
    LOGGING ( cout << " Previons "; print_OCPP_States(from_State); );
    
    if ((mjson_find(json_payload, json_payload_length, "$[0]", &tmp_char0, &tmp_int0) == MJSON_TOK_NUMBER) &&
        (mjson_find(json_payload, json_payload_length, "$[1]", &tmp_char1, &tmp_int1) == MJSON_TOK_STRING) &&
        (tmp_int0 == 1) && (tmp_int1 >= 36))
    {
        if (mjson_get_string(json_payload, json_payload_length, "$[1]", str_processing_uuid, sizeof(str_processing_uuid)) == 36) {
           
            if (tmp_char0[0] == '2') {
                /* 
                if ((mjson_find(json_payload, json_payload_length, "$[2]", &tmp_char0, &tmp_int0) == MJSON_TOK_STRING)
                    && (tmp_int0 > 0)) {
                    for (uint8_t x = 0; x < OCPP_STATES_COUNT; ++x ) {
                        if (strstr_P(tmp_char0, OCPP_sp[x].label) != NULL) {
                            if (mjson_find(json_payload, json_payload_length, "$[3]", &tmp_char1, &tmp_int1) == MJSON_TOK_OBJECT) {
                                //processing_call(OCPP_sp[x].state, str_processing_uuid, tmp_char1, tmp_int1);
                                
                                
                                return;
                            }
                        }
                    }
                    }
                    */
            } else if (tmp_char0[0] == '3') {
                if (mjson_find(json_payload, json_payload_length, "$[2]", &tmp_char1, &tmp_int1) == MJSON_TOK_OBJECT) {
                    processing_response(from_State, str_processing_uuid, tmp_char1, tmp_int1);
                    return;
                }
                
            } else if (tmp_char0[0] == '4') {
                processing_error(from_State, str_processing_uuid, json_payload, json_payload_length);
            } else {
                LOGGING( cout << PGM << PSTR("unknown message code: ") << tmp_char0[0] << EOL; );
            }
        }
    } else { cout << "Invalid UUID or code\n" << EOL; }
}



bool OCPP_Parser::check_OCPP_Frame(uint8_t *payload, uint16_t length) {
    json_payload = NULL;
    json_payload_length = 0;
//     LOGGING(
//         cout << PGM << PSTR("--check_OCPP_Frame,size: ") << length << EOL;
//         for (uint8_t x = 0; x < length; ++x) { cout << (char)payload[x]; }
//         cout << PGM << PSTR("\n---END---\n");
//     );
    
    tmp_int0 = mjson((char *)payload, length, NULL, NULL);
    if (tmp_int0 == 0) {
        LOGGING( cout << PGM << PSTR("\nNot full JSON object was buffered\n"); );
    } else if (tmp_int0 < 0) {
        LOGGING( cout << PGM << PSTR("\nInvalid JSON\n"););
    } else {
//         cout << PGM << PSTR("\n VALID JSON\n");
        json_payload = (char *)payload;
        json_payload_length = length;
        return true;
    }
    return false;
}

#define  PAIRS_BOOTNOTIFICATION_COUNT 9

const cons_string_pair Pairs_BootNotification[PAIRS_BOOTNOTIFICATION_COUNT] = {
    { str_chargePointVendor,        ocpp_def_str.val_chargePointVendor },
    { str_chargePointModel,         ocpp_def_str.val_chargePointModel },
    { str_chargePointSerialNumber,  ocpp_def_str.val_chargePointSerialNumber },
    { str_firmwareVersion,          ocpp_def_str.val_firmwareVersion },
    { str_chargeBoxSerialNumber,    ocpp_def_str.val_chargeBoxSerialNumber },
    { str_iccid,                    ocpp_def_str.val_iccid },
    { str_imsi,                     ocpp_def_str.val_imsi },
    { str_meterSerialNumber,        ocpp_def_str.val_meterSerialNumber },
    { str_meterType,                ocpp_def_str.val_meterType }
};

void OCPP_Parser::write_key_value(const char *key, const int32_t value) {
    if (key) {
        ms << '"' << PGM << key << "\":";
        ms << value;
    }
}

void OCPP_Parser::write_key_value(const cons_string_pair *sp, const bool inram) {
    if (sp) {
        ms << '"' << PGM << sp->key << "\":";
        if (inram) { ms << '"' << sp->value << '"'; }
        else { ms << '"' << PGM << sp->value << '"'; }
    }
}


void OCPP_Parser::write_comma(const uint8_t current, const uint8_t target) {
    if (current < target - 1) { ms << ','; }
}

void OCPP_Parser::write_json_start_P(const char *str) {
    ms << PGM << str << PGM << PSTR("\",{");
}

uint16_t OCPP_Parser::write_OCPP_state(OCPP_States state, bool _print) {

    ms.Enable(_print);
    ms << PGM << PSTR("[2,\"");
    write_UUID(_print);
    ms << PGM << PSTR("\",\"");

    switch (state) {
        
        case Authorize:
            write_json_start_P(str_Authorize);
            tmp_str_pair.key    = str_idTag;
            tmp_str_pair.value  = "A0-00-00-00";
            write_key_value(&tmp_str_pair, true);
        break;
    
        case BootNotification:
            write_json_start_P(str_BootNotification);
            for (uint8_t x = 0; x < PAIRS_BOOTNOTIFICATION_COUNT; ++x) {
                write_key_value(&Pairs_BootNotification[x]);
                write_comma(x, PAIRS_BOOTNOTIFICATION_COUNT);
            }
        break;
        
        case Heartbeat: // Charge points will send heartbeats roughly every 3 minutes, CMS return time
            write_json_start_P(str_Heartbeat);
        break;
        
        
        //"Available" "Preparing" "Charging" "SuspendedEVSE" "SuspendedEV"
        //"Finishing" "Reserved" "Unavailable" "Faulted" "NOT_SET"
        case StatusNotification: // reply [3,"44de7ca5-894e-4759-9351-adac869580ec",{}]
            write_json_start_P(str_StatusNotification);
            write_key_value(str_connectorId, 1);
            ms << ',';
            tmp_str_pair.key = str_errorCode;
            tmp_str_pair.value = str_NoError;
            write_key_value(&tmp_str_pair, true);
            ms << ',';
            tmp_str_pair.key = str_status;
            tmp_str_pair.value  = str_Available;
            write_key_value(&tmp_str_pair, true);
        break;
        

        

        case StartTransaction:
            write_json_start_P(str_StartTransaction);
            write_key_value(str_connectorId, 1);
            ms << ',';
            tmp_str_pair.key = str_idTag;
            tmp_str_pair.value = "A0-00-00-00";
            write_key_value(&tmp_str_pair, true);
            ms << ',';
            write_key_value(str_meterStart, 777); // WARNING W*h
            ms << ",\"";
            ms << PGM << str_timestamp << "\":";
            write_Time_to_ocpp();
            break;
            
        case StopTransaction:
            write_json_start_P(str_StopTransaction);
            write_key_value(str_meterStop, 888);
            ms << ",\"";
            ms << PGM << str_timestamp << "\":";
            write_Time_to_ocpp();
            ms << ',';
            write_key_value(str_transactionId, TransactionId);
            ms << ',';
            tmp_str_pair.key = str_reason;
            tmp_str_pair.value = "Local"; // TODO add other
            write_key_value(&tmp_str_pair, true);

            break;
        
        case MeterValues:
            // "connectorId" / "meterValue" / "timestamp"
            // "sampledValue" "value" "measurand" "unit"
            // ["measurand"] = "Energy.Active.Import.Register"; "Wh";
            // ["measurand"] = "Power.Active.Import";  ["unit"] = "W";
            // cout << PGM << str_MeterValues;
        break;
        
        case SetChargingProfile:            cout << PGM << str_SetChargingProfile; break;
        case TriggerMessage:                cout << PGM << str_TriggerMessage; break;
        case RemoteStartTransaction:
            // payload["idTag"] | String("Invalid"); payload["status"] = "Accepted";
        break;
        case RemoteStopTransaction:         cout << PGM << str_RemoteStopTransaction; break;
        case ChangeConfiguration:           cout << PGM << str_ChangeConfiguration; break;
        case GetConfiguration:
            // payload["key"] configurationKey, unknownKey, 
        break;
        case Reset:                         cout << PGM << str_Reset; break;
        case UpdateFirmware:                cout << PGM << str_UpdateFirmware; break;
        case FirmwareStatusNotification:    cout << PGM << str_FirmwareStatusNotification; break;
        case GetDiagnostics:                cout << PGM << str_GetDiagnostics; break;
        case DiagnosticsStatusNotification: cout << PGM << str_DiagnosticsStatusNotification; break;
        case UnlockConnector:               cout << PGM << str_UnlockConnector; break;
        case ClearChargingProfile:
            // reply payload["status"] = "Accepted"; Unknown
            // in: id > connectorId > chargingProfilePurpose (ChargePointMaxProfile,
            // TxDefaultProfile, TxProfile) stackLevel
        break;
        case ChangeAvailability:
            // reply payload["status"] = "Rejected", "Scheduled", "Accepted";
        break;
        // ----------------
        case CancelReservation:             cout << PGM << str_CancelReservation; break;
        case ClearCache:                    cout << PGM << str_ClearCache; break;
        case DataTransfer:                  cout << PGM << str_DataTransfer; break;
        case GetLocalListVersion:           cout << PGM << str_GetLocalListVersion; break;
        case GetCompositeSchedule:          cout << PGM << str_GetCompositeSchedule; break;
        case ReserveNow:                    cout << PGM << str_ReserveNow; break;
        case SendLocalList:                 cout << PGM << str_SendLocalList; break;
        // ----------------
        default: {
        }
        
        
        
        
        
/*        
        default: { }*/

    }
    ms << "}]";
    return ms.get_Printed();
}

void OCPP_Parser::processing_response(OCPP_States from_State, const char *uuid_str, const char *obj_str, const int obj_len) {
    // [3,"34a446bc-eea1-4b01-9cb8-b3d7c7d8196d",{}]
    // [3,"1c7b64c5-1473-4ac5-9e4b-79633b706424",{"idTagInfo":{"status":"Accepted","expiryDate":"2023-04-08T00:00:00.000Z"}}]
    // [3,"ea7e64ff-81eb-41fd-bec3-9b67bf0de98c",{"currentTime":"2022-02-17T22:16:56.388Z"}]
    // [3,"f7c064ff-1aff-4ccf-ae7f-e91dd79cb980",{"status":"Accepted","currentTime":"2022-02-11T21:15:25.350Z","interval":14400}]
    LOGGING ( cout << " processing_response "; print_OCPP_States(from_State); );
    LOGGING ( cout << " processing_response UUID:" << uuid_str; );
    LOGGING ( cout << " processing_response obj_str:" << obj_str; );
    LOGGING ( cout << " processing_response obj_len:" << obj_len << '\n'; );

    bool t_present = load_System_UnixTime(obj_str, obj_len);

    switch (from_State) {
        case Authorize:
            // {"idTagInfo":{"status":"Accepted","expiryDate":"2023-04-08T00:00:00.000Z"}}]
            if ((mjson_find(obj_str, obj_len, "$.idTagInfo", &tmp_char0, &tmp_int0) == MJSON_TOK_OBJECT)) {
                if ((mjson_find(tmp_char0, tmp_int0, "$.status", &tmp_char1, &tmp_int1) == MJSON_TOK_STRING)) {
                    for (uint8_t x = 0; x < AUTHORIZATIONSTATUS_COUNT; ++x) {
                        if (strstr_P(tmp_char1, OCPP_AuthStaPair[x].label) != NULL) {
                            Authorize_AuthorizationStatus = OCPP_AuthStaPair[x].state;
                            LOGGING( cout << PGM << PSTR("\nFOUND AuthStaPair STATUS: ") <<
                            OCPP_AuthStaPair[x].label << " (" << OCPP_AuthStaPair[x].state << ")\n"; );
                        }
                    }
                    if (mjson_get_string(tmp_char0, tmp_int0, "$.expiryDate", Authorize_expiryDate, sizeof(Authorize_expiryDate)) > 0) {
                        LOGGING ( cout << "expiryDate= " << Authorize_expiryDate << EOL;);
                    }
                }
            }
        break;

        case BootNotification: {
            // {"status":"Accepted","currentTime":"2022-02-19T01:07:53.175Z","interval":14400}]
            if (t_present) {
                if (mjson_get_number(obj_str, obj_len, "$.interval", &m_BufDouble) != 0) {
                    if ((mjson_find(obj_str, obj_len, "$.status", &tmp_char0, &tmp_int0) == MJSON_TOK_STRING)) {
                        for (uint8_t x = 0; x < REGISTRATIONSTATUS_COUNT; ++x) {
                            if (strstr_P(tmp_char0, OCPP_RegStaPair[x].label) != NULL) {
                                BootNotification_RegistrationStatus = OCPP_RegStaPair[x].state;
                                LOGGING( cout << PGM << PSTR("\nFOUND REGISTRATION STATUS: ") <<
                                OCPP_RegStaPair[x].label << " (" << OCPP_RegStaPair[x].state << ")\n"; );
                            }
                        }
                    }
                }// else { LOGGING( cout << PGM << PSTR("'interval' key not found in payload") << obj_str; ); }
            }// else { LOGGING( cout << PGM << PSTR("'currentTime' key not found in payload") << obj_str; ); }
        }
        break;

        case Heartbeat:
        break;

        case MeterValues:                   cout << PGM << str_MeterValues; break;
        case SetChargingProfile:            cout << PGM << str_SetChargingProfile; break;
        case StatusNotification:            cout << PGM << str_StatusNotification; break;
        case StartTransaction:
            cout << PGM << str_StartTransaction;
            cout << "============================ ";
            cout << "############################ ";
            
            break;
        case StopTransaction:
            cout << PGM << str_StartTransaction;
            cout << "========$$$$$$$$$$$$==== ";
            cout << "$$$$$$$$$$$$$$$$$$$$$$$$ ";
            
            break;
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
        default: {
            
        }
    }
}

void OCPP_Parser::processing_error(OCPP_States from_State, const char *uuid_str, const char *obj_str, const int obj_len) {
    LOGGING( cout << PGM << PSTR("Message 4 error - not implemented\r\n") << obj_str << EOL; );
    // [<MessageTypeId>, "<UniqueId>", "<errorCode>", "<errorDescription>", {<errorDetails>}]
    //                 const char * format = "[%d, %Q, %Q, %Q, %s]\n";
    //                 const char * errorCode_key = "$[2]";
    //                 const char * errorDescription_key = "$[3]";
    //                 const char * errorDetails_key = "$[4]";
}


}







