#pragma once


enum events {
    NONE,
    UPDATE,
    TICK_1_SEC,    
    EVSE0_STATE_RUNNING,
    EVSE0_STATE_FAULT,
    EVSE0_STATE_SELFTEST,
    EVSE0_GET_STATUS,
    LIMITER_STATE,
    LIMITER_RESET_MINMAX, // if min/max packet sent...
    NFC_CARD_READ_STATE,    
    NFC_CARD_AUTH_STATE,   
    NFC_CARDWRITE_SET,
    NFC_CARDWRITE_RESULT,
    AUTH_MODE_SET,
    GSM_MODE_SET,
    GSM_DEBUG_SET,
    GSM_MODEM_DETECTED    ,
    GSM_PINCODE_FAIL      ,
    GSM_NETWORK_DETECTED  ,
    GSM_GPRS_DETECTED     ,
    GSM_GPRS_BRINGUP      ,
    GSM_GPRS_CONNECTED    ,
    GSM_HANDLED_OK         ,
    GSM_HANDLED_ERROR      ,
    GSM_HANDLED_STRING     ,
    GSM_HANDLED_PROMPT     ,
    GSM_HANDLED_TIME_OUT   ,
    GSM_NORMAL_POWER_DOWN,          // Modem HW event
    GSM_UNDER_VOLTAGE_POWER_DOWN,   // Modem HW event
    GSM_UNDER_VOLTAGE_WARNNING,     // Modem HW event
    GSM_OVER_VOLTAGE_POWER_DOWN,    // Modem HW event
    GSM_OVER_VOLTAGE_WARNNING,      // Modem HW event    
    GSM_HANDLED_CMTI,       // New SMS arrived
    GSM_HANDLED_GCAP,       // +Complete TA Capabilities List
    GSM_HANDLED_CLOSED,       // +[CLOSED] This is bad!
    GSM_HANDLED_SMS_Ready,       // +[SMS Ready]
    GSM_HANDLED_Call_Ready,       // +[Call Ready]
    GSM_HANDLED_RDY,       // +[RDY]
    GSM_HANDLED_SEND_FAIL,       // +[SEND FAIL]
    GSM_HANDLED_DATA_ACCEPT,       // DATA ACCEPT
    GSM_HANDLED_ALREADY_CONNECT,       // +[ALREADY CONNECT]
    GSM_HANDLED_RING,       // +RING
    GSM_HANDLED_NO_CARRIER,       // +NO CARRIER
    GSM_HANDLED_CONNECT_OK,       // +CONNECT OK
    GSM_HANDLED_CONNECT_FAIL,       // +CONNECT FAIL
    GSM_HANDLED_SEND_OK,       // +SEND OK
    GSM_HANDLED_IPD,        // Handle +IPD, size: arg
    GSM_HANDLED_COPS,       // +Operator Selection
    GSM_HANDLED_STATE,      // IP state
    GSM_HANDLED_CREG,       // +Network Registration, arg - code 0..5
    GSM_HANDLED_CSQ,        // +Signal Qty RSSI, BER
    GSM_HANDLED_CMTE,       // +Temp, arg temp/int8_t
    GSM_HANDLED_SHUT_OK,    // +[SHUT OK]
    GSM_HANDLED_CPIN,       // +EnterPIN: +CPIN: READY, arg
    GSM_HANDLED_CBC         // +Battery CRG
};
