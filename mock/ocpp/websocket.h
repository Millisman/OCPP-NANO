#pragma once
#include "mcu/mcu.h"
#include <stdint.h>
#include <stdlib.h>
#include <stream/outputstream.h>

namespace ocpp {

const char s_UUID_P[] PROGMEM = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

typedef struct __attribute__((__packed__)) {
    char KeyStart[30];
    char Key[25];
} ws_key_struct;


bool websocket_check_upgrade(uint8_t *payload, uint16_t length, ws_key_struct *wsk);
void  websocket_Recv(uint8_t *data, uint16_t sz);

#define INCOMPLETE_TEXT_FRAME 0x01
#define INCOMPLETE_BINARY_FRAME 0x02
#define TEXT_FRAME 0x81
#define BINARY_FRAME 0x82
#define PING_FRAME 0x19
#define PONG_FRAME 0x1A

void Recv(uint8_t *data, uint16_t sz);

}
