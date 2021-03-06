#include "billboardtext.h"

#include <stdlib.h>

#include "mcu/timer.h"

#define ITEM_DELAY_MS 1500

namespace stream {

BillboardText::BillboardText(const uint8_t width, const uint8_t items)
    : last_update_{},
      width_(width),
      items_(items),
      current_item_(0),
      insert_position_(0) {
  buffer_ = new char[(width + 1) * items];
}

BillboardText::~BillboardText() { delete[] buffer_; }

OutputStream &BillboardText::operator<<(const ITEM &item) {
  if (item.item < items_) insert_position_ = width_ * item.item + item.item;
  return *this;
}

BillboardText &BillboardText::operator>>(OutputStream &os) {
  uint8_t i = 0;
  uint8_t offset = current_item_ * width_ + current_item_;
  while (buffer_[offset + i] && i < width_) os << buffer_[offset + i++];
  while (i++ < width_) os << " ";
  update();
  return *this;
}

void BillboardText::update() {
  const uint32_t now = mcu::Timer::millis();
  if (now - last_update_ > ITEM_DELAY_MS) {
    ++current_item_;
    if (current_item_ >= items_) current_item_ = 0;
    last_update_ = now;
  }
}

void BillboardText::clear() {
  insert_position_ = 0;
  buffer_[0] = 0;
}

void BillboardText::write(const char ch) {
  if (insert_position_ < ((width_ + 1) * items_)) {
    buffer_[insert_position_++] = ch;
    buffer_[insert_position_] = 0;
  }
}

}  // namespace stream
