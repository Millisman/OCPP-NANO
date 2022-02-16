#pragma once

#include <stdint.h>

#define EOL "\r\n"

namespace stream {
    
enum Flags { PGM, PAD_ZERO, PAD_SPACE, HEX, UPPER};

struct Spaces {
    Spaces(const uint8_t num) : num(num) {}
    const uint8_t num;
};

class OutputStream {
public:
    OutputStream(): padsc(0), flags(0) {}
    virtual ~OutputStream() {}
    
    OutputStream &operator<<(const Flags flags);
    OutputStream &operator<<(const Spaces spaces);
    OutputStream &operator<<(const char ch);
    OutputStream &operator<<(const char *str);
    OutputStream &operator<<(const  uint8_t val);
    OutputStream &operator<<(const   int8_t val);
    OutputStream &operator<<(const uint16_t val);
    OutputStream &operator<<(const  int16_t val);
    OutputStream &operator<<(const uint32_t val);
    OutputStream &operator<<(const  int32_t val);
    
protected:
    virtual void write(const char ch) = 0;
    
private:
    uint8_t padsc;
    uint8_t flags;
    void printNumber(unsigned long n, uint8_t base);
};
    
}
