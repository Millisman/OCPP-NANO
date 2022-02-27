#pragma once

#include "outputstream.h"
#include "utils/cpp.h"

namespace stream {

class BillboardText : public OutputStream {
 public:
  struct ITEM {
    explicit constexpr ITEM(uint8_t item) : item(item) {}
    const uint8_t item;
  };

  BillboardText(const uint8_t width, const uint8_t items);
  ~BillboardText();

  void clear();

  BillboardText &operator>>(OutputStream &os);
  OutputStream &operator<<(const ITEM &item);

 private:
  void update();

  virtual void write(const char ch) override;

  uint32_t last_update_;

  const uint8_t width_;
  const uint8_t items_;
  uint8_t current_item_;
  uint8_t insert_position_;
  char *buffer_;

  DISALLOW_COPY_AND_ASSIGN(BillboardText);
};

}  // namespace stream
