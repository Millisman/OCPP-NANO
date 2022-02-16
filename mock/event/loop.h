#pragma once

#include "event/event.h"
#include "event/handler.h"
#include "utils/cpp.h"
#include "utils/queue.h"


namespace event {
    
    // Basic event loop driver.
    // |dispatch| must be called regularly in order to distribute events.
    // This is a singleton; there shall be only one.
    class Loop {
        Loop();
        
    public:
        // Posts to all registered handlers on the next call to |dispatch|
        static void post(const Event &event);
        
        // Posts to all registered handlers when |dispatch| is called after
        // |ms| have elapsed
//         static void postDelayed(const Event &event, const uint32_t ms);
        
        // Remove an |event| from the event loop
        // Should be most helpful to remove delayed event before they are
        // fired.
        static void remove(const Event &event);
        
        // Must be called regularly to dispatch events
        static void dispatch();
        
        // "Handler" sub-classes do this automatically...
        static void addHandler(Handler *ph);
        
        static void removeHandler(Handler *ph);
        
    protected:
        friend class Handler;
        
    private:
        static Loop &get();
        void dispatch_impl();
        
        utils::Queue<Event> events;
        utils::Queue<Handler *> handlers;
        
        DISALLOW_COPY_AND_ASSIGN(Loop);
    };
    
}  // namespace event

