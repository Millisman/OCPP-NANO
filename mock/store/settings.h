#pragma once
#include "mcu/mcu.h"
#include <stdint.h>
#include "utils/cpp.h"
#include "utils/crc.h"
#include "stream/outputstream.h"
#include "stream/uart0stream.h"


namespace store {

enum SetCMD {S_UNK, S_READ, S_DEFS, S_WRITE};
enum AuthType: uint8_t {NOT_NEED, CONSOLE_CMD, API_CMD, AUTH_SERVER, NFC_CARD,
    invalid = 255, min = NOT_NEED, max = NFC_CARD };
enum Device_Param {P_UNK, P_AUTH, P_AMP_MAX, P_AMP, P_FB_PP, P_GSM_MOD, P_SERIAL_NUM};

typedef struct __attribute__((__packed__)) {
    uint32_t    ts;
    uint8_t     crc8;
} s_UnixTime_Block;

typedef struct __attribute__((__packed__)) {
    AuthType    auth;           // 0 - not used, or used
    uint8_t     amperageMax;    // hardware limit amperage (shutdowm cut)
    uint8_t     amperage;       // target amperage
    uint8_t     gsm_mode;       // 0 - GSM disabled...
    uint8_t     fall_back_pp;   // 0 - use standart tabe, 6-80 if not detect
    uint32_t    sn;             // hardware serial number
    uint32_t    ts;             // updated time shtamp
    uint8_t     crc8;
} s_DeviceSetup_Block;

class EepromSettings {
    stream::UartStream  cout;
    s_UnixTime_Block    TimeUnix_Block;
    s_DeviceSetup_Block DeviceSetup_Block;

    bool io_flag;
    uint32_t io_var32u;
    EepromSettings();
public:

    s_DeviceSetup_Block *get_DeviceSetup_Block() { return &DeviceSetup_Block; }
    
    static EepromSettings &init() {
        static EepromSettings settings;
        return settings;
    }
    void     save_UnixTime() { ee_IO_UnixTime(S_WRITE); }
    void     set_UnixTime(const uint32_t t) { TimeUnix_Block.ts = t; }
    void     add_UnixTime(const uint32_t t) { TimeUnix_Block.ts += t; }
    void    set_device_param(const Device_Param p, const uint32_t a);
    uint32_t get_device_param(const Device_Param p);
    uint32_t get_UnixTime() { return TimeUnix_Block.ts; }
private:
    void ee_IO_UnixTime(SetCMD o);
    void ee_IO_DeviceSetup(SetCMD o);
    DISALLOW_COPY_AND_ASSIGN(EepromSettings);
};

}
