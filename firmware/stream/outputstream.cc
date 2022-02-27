#include "mcu/mcu.h"
#include <stdlib.h>
#include <stdlib.h>
#include "stream/outputstream.h"


namespace stream {

static char OutputStream_buf[8 * sizeof(uint32_t) + 1];
    
OutputStream &OutputStream::operator<<(const Flags flag) {
    flags |= (1 << flag);
    return *this;
}

OutputStream &OutputStream::operator<<(const Spaces spaces) {
    for (size_t i = 0; i != spaces.num; ++i) *this << ' ';
    return *this;
}

OutputStream &OutputStream::operator<<(const char ch) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 2;
        printNumber(ch, 16);
    }
    write(ch);
    flags = 0;
    return *this;
}

OutputStream &OutputStream::operator<<(const char *str) {
    if (flags & (1 << Flags::PGM)) {
        while (pgm_read_byte(str)) write(pgm_read_byte(str++));
    } else {
        while (*str) write(*str++);
    }
    flags = 0;
    return *this;
}

OutputStream &OutputStream::operator<<(const  uint8_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 2;
        printNumber(val, 16);
    } else {
        padsc = 3;
        printNumber(val, 10);
    }
    flags = 0;
    return *this;
}
OutputStream &OutputStream::operator<<(const   int8_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 2;
        printNumber(val, 16);
    } else {
        padsc = 4;
        if (val < 0) {
            write('-');
            printNumber(-val, 10);
        } else { printNumber(val, 10); }
    }
    flags = 0;
    return *this;
}
OutputStream &OutputStream::operator<<(const uint16_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 4;
        printNumber(val, 16);
    } else {
        padsc = 5;
        printNumber(val, 10);
    }
    flags = 0;
    return *this;
}
OutputStream &OutputStream::operator<<(const  int16_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 4;
        printNumber(val, 16);
    } else {
        padsc = 6;
        if (val < 0) {
            write('-');
            printNumber(-val, 10);
        } else { printNumber(val, 10); }
    }
    flags = 0;
    return *this;
}
OutputStream &OutputStream::operator<<(const uint32_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 8;
        printNumber(val, 16);
    } else {
        padsc = 10;
        printNumber(val, 10);
    }
    flags = 0;
    return *this;
}
OutputStream &OutputStream::operator<<(const  int32_t val) {
    if (flags & (1 << Flags::HEX)) {
        padsc = 8;
        printNumber(val, 16);
    } else {
        padsc = 11;
        if (val < 0) {
            write('-');
            printNumber(-val, 10);
        } else { printNumber(val, 10); }
    }
    flags = 0;
    return *this;
}


void OutputStream::printNumber(unsigned long n, uint8_t base) {
    char *str = &OutputStream_buf[sizeof(OutputStream_buf) - 1];
    *str = '\0';
    if (base < 2) base = 10;    // prevent crash if called with base == 1
    char HxA = 'a';
    if (flags & (1 << Flags::UPPER)) { HxA = 'A'; }
    do {
        char c = n % base;
        n /= base;
        *--str = c < 10 ? c + '0' : c + HxA - 10;
        if (padsc) padsc--;
    } while (n);
    if (flags) {
        while (padsc--) {
            if (flags & (1 << Flags::PAD_ZERO))  { write('0'); }
            if (flags & (1 << Flags::PAD_SPACE)) { write(' '); }
        }
    }
    while (*str) { write(*str++); }
}

}
