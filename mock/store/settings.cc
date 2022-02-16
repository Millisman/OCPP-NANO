#include "mcu/mcu.h"
#include "settings.h"
#include <string.h>

#include "strings.h"

using stream::PGM;

namespace store {

s_UnixTime_Block    EEMEM   EEM_UnixTime;
s_DeviceSetup_Block EEMEM   EEM_DeviceSetup_Block;

EepromSettings::EepromSettings():
    cout(mcu::Serial0::init())
{
    cout
//     << PGM << STR_FIRMWARE_A << ' '
//     << PGM << STR_FIRMWARE_B
//     << EOL
//     << PGM << STR_FIRMWARE_VER << ' '
//     << PGM << STR_FIRMWARE_C
    << PGM << PSTR("\r\nLOAD UT:");
    ee_IO_UnixTime(S_READ);
    cout << PGM << PSTR("SETUP:");
    ee_IO_DeviceSetup(S_READ);
}

void EepromSettings::ee_IO_DeviceSetup(SetCMD o) {
    if (o == SetCMD::S_READ) {
        eeprom_read_block(&DeviceSetup_Block, &EEM_DeviceSetup_Block, sizeof(DeviceSetup_Block));
        if (DeviceSetup_Block.crc8 != utils::gencrc8((uint8_t *)&DeviceSetup_Block, sizeof(DeviceSetup_Block)-1)) {o = SetCMD::S_DEFS;}
    }
    if (o == SetCMD::S_DEFS) {
        DeviceSetup_Block.auth              = AuthType::NOT_NEED;  // 0 - not used, or used
        DeviceSetup_Block.amperageMax       = 40; // <= 80 A
        DeviceSetup_Block.amperage          = 32; // < amperageMax
        DeviceSetup_Block.gsm_mode          = 1;  // 0 - GSM disabled...
        DeviceSetup_Block.fall_back_pp      = 6;  // 0 - use standart tabe, 6-80 if not detect
        DeviceSetup_Block.sn                = 0;  // hardware serial number
        o = SetCMD::S_WRITE;
    }
    if (o == SetCMD::S_WRITE) {
        DeviceSetup_Block.ts = TimeUnix_Block.ts;
        DeviceSetup_Block.crc8 = utils::gencrc8((uint8_t *)&DeviceSetup_Block, sizeof(DeviceSetup_Block)-1);
        eeprom_write_block(&DeviceSetup_Block, &EEM_DeviceSetup_Block, sizeof(DeviceSetup_Block));
    }
}

void EepromSettings::ee_IO_UnixTime(SetCMD o) {
    if (o == SetCMD::S_READ) {
        eeprom_read_block(&TimeUnix_Block, &EEM_UnixTime, sizeof(TimeUnix_Block));
        if (TimeUnix_Block.crc8 != utils::gencrc8((uint8_t *)&TimeUnix_Block, sizeof(TimeUnix_Block)-1)) {o = SetCMD::S_DEFS;}
    }
    if (o == SetCMD::S_DEFS) {
        TimeUnix_Block.ts = 1640293861;
        o = SetCMD::S_WRITE;
    }
    if (o == SetCMD::S_WRITE) {
        TimeUnix_Block.crc8 = utils::gencrc8((uint8_t *)&TimeUnix_Block, sizeof(TimeUnix_Block)-1);
        eeprom_write_block(&TimeUnix_Block, &EEM_UnixTime, sizeof(TimeUnix_Block));
    }
}

void EepromSettings::set_device_param(const Device_Param p, const uint32_t a) {
    io_flag = false;
    switch (p) {
        case P_AUTH:
            if ((a >= AuthType::min) && (a <= AuthType::max) && (a != DeviceSetup_Block.auth)) {
                DeviceSetup_Block.auth = (AuthType)a;
                io_flag = true;
            }
            break;
        case P_AMP_MAX:
            if ((a >= 11) && (a <= 90) && (a != DeviceSetup_Block.amperageMax)) {
                DeviceSetup_Block.amperageMax = a;
                io_flag = true;
            }
            break;
        case P_AMP:
            if ((a >= 6) && (a <= 80) && (a != DeviceSetup_Block.amperage)) {
                DeviceSetup_Block.amperage = a;
                io_flag = true;
            }
            break;
        case P_FB_PP:
            if ((((a >= 6) && (a <= 80)) || (a == 0)) && (a != DeviceSetup_Block.fall_back_pp)) {
                DeviceSetup_Block.fall_back_pp = a;
                io_flag = true;
            }
            break;
        case P_GSM_MOD:
            DeviceSetup_Block.gsm_mode = a;
            io_flag = true;
            break;
        case P_SERIAL_NUM:
            if ((a > 0) && (a != DeviceSetup_Block.sn)) {
                DeviceSetup_Block.sn = a;
                io_flag = true;
            }
            break;
        default: {}
    };
    if (io_flag) ee_IO_DeviceSetup(S_WRITE);
}

uint32_t EepromSettings::get_device_param(const Device_Param p) {
    io_var32u = 0;
    switch (p) {
        case P_AUTH:        io_var32u = DeviceSetup_Block.auth;          break;
        case P_AMP_MAX:     io_var32u = DeviceSetup_Block.amperageMax;   break;
        case P_AMP:         io_var32u = DeviceSetup_Block.amperage;      break;
        case P_FB_PP:       io_var32u = DeviceSetup_Block.fall_back_pp;  break;
        case P_GSM_MOD:     io_var32u = DeviceSetup_Block.gsm_mode;      break;
        case P_SERIAL_NUM:  io_var32u = DeviceSetup_Block.sn;            break;
        default: {}
    };
    return io_var32u;
}

}
