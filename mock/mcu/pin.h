#pragma once
#include "mcu/mcu.h"

// This us usually how one would create a "Pin"...
#define MAKEPIN(a, n, d) mcu::Pin(&(PIN##a), (1 << n), mcu::Pin::PIN_##d)


namespace mcu {

class Pin {
 public:
  enum PinDirection { PIN_IN, PIN_IN_PULLUP, PIN_OUT };

  Pin(volatile uint8_t *reg_pin, const uint8_t b, const PinDirection d);

//   uint16_t analogRead() const;

  Pin &operator=(const uint8_t rhs);
  bool operator!() const;

 private:
  volatile uint8_t *const reg_pin;
  const uint8_t b;
};

}  // namespace mcu
