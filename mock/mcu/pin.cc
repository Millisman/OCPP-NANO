#include "mcu/mcu.h"
#include "pin.h"

#define _AVR_REG(addr)

#define _PIN _AVR_REG(reg_pin)
#define _DDR _AVR_REG(reg_pin + 1)
#define _PORT _AVR_REG(reg_pin + 2)


namespace mcu {
    
    Pin::Pin(volatile uint8_t *reg_pin, const uint8_t b, const PinDirection d):
    reg_pin(reg_pin), b(b)
    {
//         _DDR &= ~b;
//         
//         if (d == PIN_OUT) _DDR |= b;
//         else if (d == PIN_IN_PULLUP)
//             _PORT |= b;
    }
    
//     uint16_t Pin::analogRead() const {
//         ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
//         ADMUX = (1 << REFS0) | bit2num(b);
//         ADCSRA |= (1 << ADSC);
//         while (ADCSRA & (1 << ADSC)) {
//         };
//         return ADCW;
//     }
    
    Pin &Pin::operator=(const uint8_t rhs) {
//         if (rhs)
//             _PORT |= b;
//         else
//             _PORT &= ~b;
        
        return *this;
    }
    
    bool Pin::operator!() const { return (_PIN & b) ? false : true; }
    
}  // namespace board

