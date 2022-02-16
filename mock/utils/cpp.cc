#include <stdint.h>
#include <stdlib.h>

void *operator new(const size_t size) { return malloc(size); }

void operator delete(void *p, size_t) { free(p); }
void operator delete(void *p) { free(p); }

void *operator new[](const size_t size) { return malloc(size); }

void operator delete[](void *p) { free(p); }
void operator delete[](void *p, size_t) { free(p); }

extern "C" {
    __extension__ typedef int __guard __attribute__((mode(__DI__)));
    
    int __cxa_guard_acquire(__guard *g) { return !*reinterpret_cast<uint8_t *>(g); }
    
    void __cxa_guard_release(__guard *g) { *reinterpret_cast<uint8_t *>(g) = 1; }
    
    void __cxa_guard_abort(__guard *) {}
    
    void __cxa_pure_virtual() {
        while (1) {
        }
    }
    
    int __attribute__((weak)) atexit(void (*)(void)) { return 0; }
}
