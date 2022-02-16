#pragma once

#include <stdint.h>
#include "stream/outputstream.h"
#include "stream/uart0stream.h"

namespace utils {

void Hexdump(stream::OutputStream& out, const uint8_t RowSize, bool ShowAscii, const void* mData, const uint8_t mLength);
    
}
