#ifndef Rect_HPP
#define Rect_HPP

#include <cassert>
#include <glm/glm.hpp>

template<typename T=float>
class Rectangle {
public:
    T m_left;
    T m_right;
    T m_top; 
    T m_bottom;    

    constexpr Rectangle(T cx=0, T cy=0, T width=0, T height=0) noexcept :
        m_left(cx-width/2), m_right(cx+width/2), m_top(cy+height/2), m_bottom(cy-height/2) {

        assert(width >= 0 && height >= 0 && "ERROR: width and height need to be >= 0.");
    }

    constexpr void resize(T cx, T cy, T width, T height) noexcept {
        m_left = cx-width/2;
        m_right = cx+width/2;
        m_top = cy+height/2;
        m_bottom = cy-height/2;

        assert(width >= 0 && height >= 0 && "ERROR: width and height need to bed = 0.");
    }

    constexpr void resizeWithCorners(T left, T bottom, T right, T top) noexcept {
        m_left = left;
        m_right = right;
        m_top = top;
        m_bottom = bottom;

        assert(left < right && bottom < top && "ERROR: need left <= right and bottom <= top.");
    }

    constexpr void resizeLeftRightBottomTop(T left, T right, T bottom, T top) noexcept {
        m_left = left;
        m_right = right;
        m_top = top;
        m_bottom = bottom;

        assert(left < right && bottom < top && "ERROR: need left <= right and bottom <= top.");
    }


    constexpr T getLeft() const noexcept {
        return m_left;
    }

    constexpr T getRight() const noexcept {
        return m_right;
    }

    constexpr T getTop() const noexcept {
        return m_top;
    }
 
    constexpr T getBottom() const noexcept {
        return m_bottom;
    }

    constexpr T getWidth() const noexcept {
        return m_right-m_left;
    }

    constexpr T getHeight() const noexcept {
        return m_top-m_bottom;
    }

    constexpr glm::vec2 getCenter() const noexcept {
        return glm::vec2(m_left + getWidth()/2, m_bottom + getHeight()/2);
    }

    constexpr glm::vec2 getSize() const noexcept {
        return glm::vec2(getWidth(), getHeight());
    }

    constexpr bool contains(const Rectangle& other) const noexcept {
        return  m_left <= other.m_left && other.m_right <= m_right &&
                m_bottom <= other.m_bottom && other.m_top <= m_top;
    }

    //m_left and m_bottom are "lower or equal to" and m_right and m_top are "strictly greter to"
    //in order to avoiding that a point, located in the edge, be detected in two rectangles next to each other
    constexpr bool contains(const glm::vec2& vec) const noexcept {
        return  m_left <= vec.x && vec.x < m_right &&
                m_bottom <= vec.y && vec.y < m_top;
    }

    constexpr bool contains( T x, T y) const noexcept {
        return  m_left <= x && x < m_right &&
                m_bottom <= y && y < m_top;
    }

    constexpr bool intersects(const Rectangle& other) const noexcept {
        return !(m_left > other.m_right || m_right < other.m_left ||
                m_bottom > other.m_top || m_top < other.m_bottom);
    }

    Rectangle& operator=(const Rectangle& other) {
        m_left = other.m_left;
        m_right = other.m_right;
        m_top = other.m_top;
        m_bottom = other.m_bottom;

        return *this;

    }
};

#endif