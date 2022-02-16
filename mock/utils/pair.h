#pragma once

namespace utils {
    
    template <class T1, class T2>
    class Pair {
    public:
        T1 first;
        T2 second;
        
        Pair(const T1 &first, const T2 &second) : first(first), second(second) {}
    };
    
}  // namespace utils

