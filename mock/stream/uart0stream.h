#pragma once

#include "mcu/serial.h"
#include "outputstream.h"

namespace stream {

class UartStream : public OutputStream {
public:
    explicit UartStream(mcu::Serial0 &Uart);
    UartStream &operator>>(char &ch);
    
private:
    void write(const char ch) override;
    mcu::Serial0 &uart;
};
    
}
