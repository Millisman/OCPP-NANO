#include "scrollingtext.h"

#include <stdlib.h>

#include "mcu/timer.h"

#define SCROLL_DELAY_MS 300

namespace stream {

ScrollingText::ScrollingText(const uint8_t size, const uint8_t width)
    : last_update{}, size(size), width(width), length(0), offset(0) {
  buffer = new char[size];
}

ScrollingText::~ScrollingText() { delete[] buffer; }

void ScrollingText::clear() { length = 0; }

void ScrollingText::setWidth(const uint8_t width) { this->width = width; }

ScrollingText &ScrollingText::operator>>(OutputStream &os) {
  if (length > width) *this << " ~ ";

  if (length <= width || offset >= length) offset = 0;

  uint8_t chars = width;
  uint8_t idx = offset;
  buffer[length] = 0;

  while (chars--) {
    if (length >= width || idx < length) {
      os << buffer[idx++];
      if (length >= width && idx >= length) idx = 0;
    } else {
      os << ' ';
    }
  }

  update();
  return *this;
}

void ScrollingText::update() {
  const uint32_t now = mcu::Timer::millis();
  if (now - last_update > SCROLL_DELAY_MS) {
    ++offset;
    last_update = now;
  }
}

void ScrollingText::write(const char ch) {
  if (length < (size - 1)) buffer[length++] = ch;
}

}  // namespace stream
