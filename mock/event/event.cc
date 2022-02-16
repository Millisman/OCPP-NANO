#include "event/event.h"

namespace event {
    
    Event::Event(const uint8_t id, const uint8_t param)
    : id(id), param(param) {} // , delay(0), posted(0)
    
    bool Event::operator==(const Event &rhs) const { return this->id == rhs.id; }
    
}  // namespace event

