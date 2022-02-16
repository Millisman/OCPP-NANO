#include "dump.h"
#include "bcd.h"

namespace utils {

using  stream::Flags;
    
void Hexdump(stream::OutputStream& out, const uint8_t RowSize, bool ShowAscii, const void* mData, const uint8_t mLength) {
    uint8_t n;
    const char sep[] = " | ";
    out << EOL;
    for (uint8_t i = 0; i < mLength; i += RowSize) {
        n = i;
        out << sep;
        for (uint8_t j = 0; j < RowSize; ++j) {
            if (i + j < mLength) {
                n = ((uint8_t*)mData)[i + j];
                out << Flags::PAD_ZERO << Flags::UPPER << Flags::HEX << n;
            } else out << "   ";
        }
        if (ShowAscii) {
            out << sep;
            for (uint8_t j = 0; j < RowSize; ++j) {
                if (i + j < mLength) {
                    if (((uint8_t*)mData)[i + j] >= ' ' && ((uint8_t*)mData)[i + j] <= '~') {
                        out << ((char*)mData)[i + j];
                    } else out << '.';
                }
            }
        }
        out << EOL;
    }
    out << EOL;
}
    
}


