#include "event/loop.h"
#include "mcu/timer.h"

namespace event {

    void Loop::post(const Event &event) { get().events.push(event); }

    void Loop::remove(const Event &event) {
        auto &events = get().events;
        for (auto it = events.begin(); it != events.end();) {
            if (event == *it)
                events.erase(it++);
            else
                ++it;
        }
    }
    
    void Loop::addHandler(Handler *ph) { get().handlers.push(ph); }
    
    void Loop::removeHandler(Handler *ph) {
        auto &handlers = get().handlers;
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            if (*it == ph) {
                handlers.erase(it);
                break;
            }
        }
    }
    
    void Loop::dispatch() { get().dispatch_impl(); }
    
    Loop &Loop::get() {
        static Loop loop;
        return loop;
    }

    Loop::Loop() {}

    void Loop::dispatch_impl() {
        for (auto it = events.begin(); it != events.end();) {
            const Event &event = *it;
                for (auto handler : handlers) handler->onEvent(event);
                events.erase(it++);
        }
    }
}  // namespace event

