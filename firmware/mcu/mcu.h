#pragma once

#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/delay.h>
#define SEI sei()
#else
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <iostream>
#include "mcu_iom644pa.h"
#define PSTR
#define SEI
#define PROGMEM
#define memcpy_P memcpy
#define strstr_P strstr
#define strcasestr_P strcasestr
#define EEMEM
#define strlen_P strlen
#include <bits/stdint-uintn.h>
#include <functional>
// #include <iostream>
#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */
#include <time.h>
#include <assert.h>
#include <sys/time.h>
inline uint8_t  pgm_read_byte(const uint8_t * _address_short)  {return *_address_short;}
inline     char pgm_read_byte(const char * _address_short)     {return *_address_short;}
inline uint16_t pgm_read_word(const uint16_t * _address_short) {return *_address_short;}
inline uint32_t pgm_read_dword(const uint32_t * _address_short){return *_address_short;}


#define EEPROM_SIZE 4096

static char EEPROM[EEPROM_SIZE];

inline void _test_eeprom_reset() { memset( EEPROM, 0xFE, sizeof(EEPROM )); }

inline void eeprom_write_block( const void * src, void* dst, size_t n ) {
    (void)src;
    (void)dst;
    (void)n;
}

inline void eeprom_read_block(  void * dest, const void * source, size_t n ) {
    (void)source;
    (void)dest;
    (void)n;
}



#endif

namespace mcu {

}
