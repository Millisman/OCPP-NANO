#include "serial.h"
#include <iostream>
#include <string>

namespace mcu {

Serial0::Serial0() { }

void Serial0::write(const uint8_t data) {
    std::cout << (char)data;
}

}
