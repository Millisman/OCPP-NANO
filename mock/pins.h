#pragma once

#include "mcu/pin.h"

#define PIN_LED_RED     MAKEPIN(A, 0, OUT) // PA0 (OUT), pull down 10K, driving LED LOAD
#define PIN_LED_GREEN   MAKEPIN(A, 1, OUT) // PA1 (OUT), pull down 10K, driving LED LOAD
#define PIN_LED_BLUE    MAKEPIN(A, 2, OUT) // PA2 (OUT), pull down 10K, driving LED LOAD
