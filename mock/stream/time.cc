#include "time.h"

namespace stream {

Time::Time(const uint8_t hh, const uint8_t mm) : hh(hh), mm(mm) {}

OutputStream &operator<<(OutputStream &out, const Time &time) {
  return out << stream::PAD_ZERO << time.hh << ':' << stream::PAD_ZERO << time.mm;
}

}  // namespace stream
