#pragma once


namespace utils {
    
    template <typename T>
    T min(T a, T b) {
        if (b < a) return b;
        return a;
    }
    
    template <typename T>
    T max(T a, T b) {
        if (b > a) return b;
        return a;
    }
    
    template <typename T>
    T square_root(const T &val) {
        if (val < 2) return val;
        
        T min = 1;
        T max = val / 2 + 1;
        
        while ((max - min) > 1) {
            const T test = (max - min) / 2 + min;
            const T sq = test * test;
            
            if (sq == val)
                return test;
            else if (sq > val)
                max = test;
            else
                min = test;
        }
        
        return min;
    }
    
}  // namespace utils

