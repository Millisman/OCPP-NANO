#pragma once

#include "event/event.h"
#include "utils/cpp.h"


namespace event {
    
    // Abstract base clas for an event receiver. Derived classes will
    // be automatically registered with the event looper.
    class Handler {
    public:
        Handler();
        virtual ~Handler();
        
        virtual void onEvent(const Event &event) = 0;
        
        DISALLOW_COPY_AND_ASSIGN(Handler);
    };
    
}  // namespace event

