#include <stddef.h>
#include <string.h>
#include "sha1.h"
#include "websocket.h"
#include "Base64.h"
#include <stream/uart0stream.h>
#include <stdlib.h>

#define  LOG_ON

namespace ocpp {  

using stream::PGM;

#ifdef LOG_ON
stream::UartStream cout(mcu::Serial0::init());    
#define LOGGING(x) cout << EOL << uint16_t(__LINE__) << ' ' << __func__ << ' '; x
#else
#define LOGGING(x)
#endif
    
    
bool websocket_check_upgrade(uint8_t *payload, uint16_t length, ws_key_struct *wsk) {
    (void)length;
    LOGGING( cout << PGM << PSTR("find.. HTTP/1.1\r\n"));
    if ((strstr_P((const char *)payload, PSTR("HTTP/1.1 101")) != NULL) || (strstr_P((const char *)payload, PSTR("HTTP/1.0 101")) != NULL)) {
        LOGGING(cout << PGM << PSTR(" ..valid status  \n"););
        if (strcasestr_P((const char *)payload, PSTR("Upgrade:")) != NULL) {
            if (strcasestr_P((const char *)payload, PSTR("websocket")) != NULL) {
                LOGGING(cout << PGM << PSTR(" ..valid REQUEST_HAS_UPGRADE  \n"););
                if (strcasestr_P((const char *)payload, PSTR("Connection:")) != NULL) {
                    if (strcasestr_P((const char *)payload, PSTR("upgrade")) != NULL) {
                        LOGGING(cout << PGM << PSTR(" ..valid REQUEST_HAS_CONNECTION  \n"););                        
                        if (strcasestr_P((const char *)payload, PSTR("Sec-WebSocket-Accept:")) != NULL) {
                            LOGGING(cout << PGM << PSTR(" ..valid Sec-WebSocket-Accept  \n"););
                            Sha1Class Sha1; // ALLOC 184B
                            Sha1.init();
                            for (uint8_t a = 0; a < strlen(wsk->Key); ++a) { Sha1.write(wsk->Key[a]); }
                            char m;
                            char *mp = (char *)&s_UUID_P;
                            while ((m = pgm_read_byte(mp++))) { Sha1.write(m); }
                            uint8_t *hash = Sha1.result();
                            wsk->KeyStart[base64_encode(wsk->KeyStart, (char *)hash, 20)] = '\0';
                            if (strcasestr((const char *)payload, wsk->KeyStart) != NULL) {
                                LOGGING(cout << PGM << PSTR(" ..valid REQUEST_VALID_ACCEPT  \n"););
                                return true;
                            }                            
                        }
                    }
                }
            }
        }
    }
    return false;
}

void  websocket_Recv(uint8_t *data, uint16_t sz) {
    if (sz < 2) { return; }
    uint8_t fin =  (data[0] & 0x80) == 0x80 ? 1 : 0; // msg_fin = (in_buffer[0] >> 7) & 0x01;
    uint8_t opcode = data[0] & 0x0F; 
    if ((data[1] & 0x80) != 0x0) { return; } // WS_COMPLETE_FRAME_MASKED_ERR
    uint8_t msg_masked = (data[1] >> 7) & 0x01;
    uint8_t payload_offset = 2;
    uint8_t length_field = data[1] & (~0x80);
    uint16_t payload_length = 0;
    if(length_field <= 125) {
        payload_length = length_field;
    } else if(length_field == 126) { // msglen is 16bit!
        payload_length = ( (data[2] << 8) | (data[3]) );
        payload_offset += 2;
    } else return;
    if (sz < payload_length + payload_offset) { return; } // INCOMPLETE_FRAME
    if (msg_masked) {
        uint32_t mask = *((uint32_t *)(data + payload_offset));
        payload_offset += 4;
        uint8_t *c = data + payload_offset;
        for(uint16_t i=0; i < payload_length; i++) { // unmask data:
            c[i] = c[i] ^ ((uint8_t *)(&mask))[i % 4];
        }
    }

    cout << (char *)data << EOL;
}

}
