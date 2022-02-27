#include "mcu/mcu.h"
#include "ocpp_strings.h"


const char WSH_GET[]  PROGMEM = "GET ";
const char WSH_HTTP[] PROGMEM = " HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Protocol: ocpp1.6\r\nHost: ";
const char WSH_KEY[]  PROGMEM = "\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: ";
const char WSH_EOL2[] PROGMEM = "\r\n\r\n";

const char str_Authorize[]               PROGMEM = "Authorize";
const char str_BootNotification[]        PROGMEM = "BootNotification";
const char str_Heartbeat[]               PROGMEM = "Heartbeat";
const char str_MeterValues[]             PROGMEM = "MeterValues";
const char str_SetChargingProfile[]      PROGMEM = "SetChargingProfile";
const char str_StatusNotification[]      PROGMEM = "StatusNotification";
const char str_StartTransaction[]        PROGMEM = "StartTransaction";
const char str_StopTransaction[]         PROGMEM = "StopTransaction";
const char str_TriggerMessage[]          PROGMEM = "TriggerMessage";
const char str_GetConfiguration[]        PROGMEM = "GetConfiguration";
const char str_Reset[]                   PROGMEM = "Reset";
const char str_UpdateFirmware[]          PROGMEM = "UpdateFirmware";
const char str_GetDiagnostics[]          PROGMEM = "GetDiagnostics";
const char str_UnlockConnector[]         PROGMEM = "UnlockConnector";
const char str_ClearChargingProfile[]    PROGMEM = "ClearChargingProfile";
const char str_ChangeAvailability[]      PROGMEM = "ChangeAvailability";
const char str_CancelReservation[]       PROGMEM = "CancelReservation";
const char str_ClearCache[]              PROGMEM = "ClearCache";
const char str_DataTransfer[]            PROGMEM = "DataTransfer";
const char str_GetLocalListVersion[]     PROGMEM = "GetLocalListVersion";
const char str_GetCompositeSchedule[]    PROGMEM = "GetCompositeSchedule";
const char str_ReserveNow[]              PROGMEM = "ReserveNow";
const char str_SendLocalList[]           PROGMEM = "SendLocalList";
const char str_Pending[]                 PROGMEM = "Pending";
const char str_Rejected[]                PROGMEM = "Rejected";
const char str_Inoperative[]             PROGMEM = "Inoperative";
const char str_Operative[]               PROGMEM = "Operative";
const char str_Undefined[]               PROGMEM = "Undefined";
const char str_Scheduled[]               PROGMEM = "Scheduled";
const char str_Available[]               PROGMEM = "Available";
const char str_Preparing[]               PROGMEM = "Preparing";
const char str_Charging[]                PROGMEM = "Charging";
const char str_SuspendedEVSE[]           PROGMEM = "SuspendedEVSE";
const char str_SuspendedEV[]             PROGMEM = "SuspendedEV";
const char str_Finishing[]               PROGMEM = "Finishing";
const char str_Reserved[]                PROGMEM = "Reserved";
const char str_Unavailable[]             PROGMEM = "Unavailable";
const char str_Faulted[]                 PROGMEM = "Faulted";
const char str_GroundFailure[]           PROGMEM = "GroundFailure";
const char str_HighTemperature[]         PROGMEM = "HighTemperature";
const char str_InternalError[]           PROGMEM = "InternalError";
const char str_LocalListConflict[]       PROGMEM = "LocalListConflict";
const char str_NoError[]                 PROGMEM = "NoError";
const char str_OtherError[]              PROGMEM = "OtherError";
const char str_OverCurrentFailure[]      PROGMEM = "OverCurrentFailure";
const char str_OverVoltage[]             PROGMEM = "OverVoltage";
const char str_PowerMeterFailure[]       PROGMEM = "PowerMeterFailure";
const char str_PowerSwitchFailure[]      PROGMEM = "PowerSwitchFailure";
const char str_ReaderFailure[]           PROGMEM = "ReaderFailure";
const char str_ResetFailure[]            PROGMEM = "ResetFailure";
const char str_UnderVoltage[]            PROGMEM = "UnderVoltage";
const char str_WeakSignal[]              PROGMEM = "WeakSignal";
const char str_Accepted[]                PROGMEM = "Accepted";
const char str_Blocked[]                 PROGMEM = "Blocked";
const char str_Expired[]                 PROGMEM = "Expired";
const char str_Invalid[]                 PROGMEM = "Invalid";
const char str_ConcurrentTx[]            PROGMEM = "ConcurrentTx";
const char str_DeAuthorized[]            PROGMEM = "DeAuthorized";
const char str_EmergencyStop[]           PROGMEM = "EmergencyStop";
const char str_EVDisconnected[]          PROGMEM = "EVDisconnected";
const char str_HardReset[]               PROGMEM = "HardReset";
const char str_Local[]                   PROGMEM = "Local";
const char str_Other[]                   PROGMEM = "Other";
const char str_PowerLoss[]               PROGMEM = "PowerLoss";
const char str_Reboot[]                  PROGMEM = "Reboot";
const char str_Remote[]                  PROGMEM = "Remote";
const char str_SoftReset[]               PROGMEM = "SoftReset";
const char str_UnlockCommand[]           PROGMEM = "UnlockCommand";
const char str_ConnectorLockFailure[]    PROGMEM = "ConnectorLockFailure";
const char str_EVCommunicationError[]    PROGMEM = "EVCommunicationError";
const char str_FirmwareStatusNotification[]      PROGMEM = "FirmwareStatusNotification";
const char str_DiagnosticsStatusNotification[]   PROGMEM = "DiagnosticsStatusNotification";
const char str_RemoteStartTransaction[]          PROGMEM = "RemoteStartTransaction";
const char str_RemoteStopTransaction[]           PROGMEM = "RemoteStopTransaction";
const char str_ChangeConfiguration[]             PROGMEM = "ChangeConfiguration";
const char str_status[]      PROGMEM = "status";
const char str_currentTime[] PROGMEM = "currentTime";
const char str_interval[]    PROGMEM = "interval";
const char str_idTag[]       PROGMEM = "idTag";
const char str_connectorId[] PROGMEM = "connectorId";
const char str_errorCode[]   PROGMEM = "errorCode";

const char     str_http_11[]  PROGMEM = "HTTP/1.1 101";
const char     str_http_10[]  PROGMEM = "HTTP/1.0 101";
const char str_SecWsAccept[]  PROGMEM = "Sec-WebSocket-Accept:";
const char   str_websocket[]  PROGMEM = "websocket";
const char   str_Connection[] PROGMEM = "Connection:";
const char     str_upgrade[]  PROGMEM = "upgrade";
const char       str_UUID[]   PROGMEM = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";


const char        str_chargePointVendor[] PROGMEM = "chargePointVendor";
const char         str_chargePointModel[] PROGMEM = "chargePointModel";
const char  str_chargePointSerialNumber[] PROGMEM = "chargePointSerialNumber";
const char    str_chargeBoxSerialNumber[] PROGMEM = "chargeBoxSerialNumber";
const char          str_firmwareVersion[] PROGMEM = "firmwareVersion";
const char                    str_iccid[] PROGMEM = "iccid";
const char                     str_imsi[] PROGMEM = "imsi";
const char        str_meterSerialNumber[] PROGMEM = "meterSerialNumber";
const char                str_meterType[] PROGMEM = "meterType";

const char   str_reason[]        PROGMEM = "reason";
const char   str_transactionId[] PROGMEM = "transactionId";
const char   str_meterStart[]    PROGMEM = "meterStart";
const char   str_meterStop[]     PROGMEM = "meterStop";
const char   str_timestamp[]     PROGMEM = "timestamp";










