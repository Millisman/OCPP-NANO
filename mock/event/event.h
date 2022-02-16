#pragma once

#include <stdbool.h>
#include <stdint.h>


namespace event {
    
    class Event {
    public:
        explicit Event(const uint8_t id, const uint8_t param = 0);
        bool operator==(const Event &rhs) const;
        const uint8_t id;
        const uint8_t param;
    };
    
}  // namespace event

