#pragma once

#include "stream/outputstream.h"
#include "utils/cpp.h"

namespace stream {

class ScrollingText : public OutputStream {
 public:
  ScrollingText(const uint8_t size, const uint8_t width);
  ~ScrollingText();

  void setWidth(const uint8_t width);
  void clear();

  ScrollingText &operator>>(OutputStream &os);

 private:
  void update();

  void write(const char ch) override;

  uint32_t last_update;

  const uint8_t size;
  uint8_t width;
  uint8_t length;
  uint8_t offset;
  char *buffer;

  DISALLOW_COPY_AND_ASSIGN(ScrollingText);
};

}  // namespace stream
