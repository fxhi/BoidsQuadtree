#ifndef LINE_HPP
#define LINE_HPP

template<typename T>
struct Line {
    Line(T x1, T y1, T x2, T y2) : x1(x1), y1(y1), x2(x2), y2(y2) {
        // Nothing
    }
    
    T x1, y1, x2, y2;
};

#endif