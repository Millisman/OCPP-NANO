#include "uart0stream.h"

namespace stream {
    
UartStream::UartStream(mcu::Serial0 &Uart):
uart(Uart)
{ }

void UartStream::write(const char ch) {
    uart.write(ch);
}
    
}
