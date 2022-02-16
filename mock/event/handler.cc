
#include "event/handler.h"
#include "event/loop.h"


namespace event {
    
    Handler::Handler() { Loop::get().addHandler(this); }
    
    Handler::~Handler() {}
    
}  // namespace event

