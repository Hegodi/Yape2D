#pragma once
#include <cmath>
#include <iostream>

namespace yape2d
{

    // Template class for 2D vector
    template <typename T>
    class Vector2D {
    public:
        T x, y;

        // Constructors
        Vector2D() : x(0), y(0) {}
        Vector2D(T x, T y) : x(x), y(y) {}

        T length2() const
        {
            return x * x + y * y;
        }

        T length () const
        {
            return std::sqrt(length2());
        }

        Vector2D normalized() const
        {
            T len = length();
            return (len > 0) ? Vector2D(x / len, y / len) : Vector2D(0, 0);
        }

        T dot(const Vector2D& other) const
        {
            return x * other.x + y * other.y;
        }

        Vector2D operator+(const Vector2D& other) const
        {
            return Vector2D(x + other.x, y + other.y);
        }

        Vector2D operator-(const Vector2D& other) const
        {
            return Vector2D(x - other.x, y - other.y);
        }

        Vector2D operator*(T scalar) const
        {
            return Vector2D(x * scalar, y * scalar);
        }

        Vector2D operator/(T scalar) const
        {
            return (scalar != 0) ? Vector2D(x / scalar, y / scalar) : Vector2D(0, 0);
        }

        Vector2D& operator+=(const Vector2D& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2D& operator-=(const Vector2D& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2D& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2D& operator/=(T scalar)
        {
            if (scalar != 0)
            {
                x /= scalar;
                y /= scalar;
            }
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2D& v)
        {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }
    };

    using vec2f = Vector2D<float>;
    using vec2i = Vector2D<int>;
}