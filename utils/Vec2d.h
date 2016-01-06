#pragma once

#include "Matrix3x2.h"

template<typename T>
struct Vec2d
{
    Vec2d() { this->x = 0; this->y = 0; }

    Vec2d(const T &x, const T &y) { this->x = x; this->y = y; }
    
    T x, y;

    Vec2d(const Vec2d &b) {
        x = b.x;
        y = b.y;
    }

    Vec2d operator=(const Vec2d &b) {
        x = b.x;
        y = b.y;
        return *this;
    }

    Vec2d operator+(const Vec2d &b) const {
        return Vec2d(x + b.x, y + b.y);
    }

    Vec2d operator+=(const Vec2d &b) {
        x += b.x;
        y += b.y;
        return *this;
    }
    bool operator==(const Vec2d &b) const {
        return (x == b.x && y == b.y);
    }
    bool operator!=(const Vec2d &b) const {
        return !(*this == b);
    }
    Vec2d operator-(const Vec2d &b) const {
        return Vec2d(x - b.x, y - b.y);
    }
    Vec2d operator*(const double &scale) const {
        return Vec2d(x*scale, y*scale);
    }
    Vec2d operator*=(const double &scale) {
        x *= scale;
        y *= scale;
        return *this;
    }
    T Abs() const {
        return sqrt(x*x + y*y);
    }
    Vec2d dotMultiply(const Vec2d& b) const {
        return Vec2d(x*b.x, y*b.y);
    }
    void Scaled(const double &scale) {
        x *= scale;
        y *= scale;
    }
    void Update(const T &x, const T &y) {
        this->x = x;
        this->y = y;
    }
    void SetZero(void) {
        x = 0;
        y = 0;
    }
    Vec2d operator*(const Matrix3x2<T> &matrix) const {
        Vec2d newVec;
        newVec.x = x * matrix._11 + y * matrix._21 + matrix._31;
        newVec.y = x * matrix._12 + y * matrix._22 + matrix._32;
        return newVec;
    }
};

typedef Vec2d<int> Vec2dInt;
typedef Vec2d<unsigned int> Vec2dUint;
typedef Vec2d<float> Vec2dFloat;