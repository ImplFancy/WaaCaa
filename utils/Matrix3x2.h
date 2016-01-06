#pragma once

#include <cmath>

template<typename T>
struct Matrix3x2
{
    Matrix3x2 operator*(const Matrix3x2 &b) {
        _11 = _11 * b._11 + _12 * b._21;
        _12 = _11 * b._12 + _12 * b._22;
        _21 = _21 * b._11 + _22 * b._21;
        _22 = _21 * b._12 + _22 * b._22;
        _31 = _31 * b._11 + _32 * b._21 + b._31;
        _32 = _31 * b._12 + _32 * b._22 + b._32;
        return *this;
    }

    static Matrix3x2 Identity()
    {
        Matrix3x2 matrix;
        matrix._11 = matrix._22 = 1;
        return matrix;
    }

    // 2d move transformation matrix
    static Matrix3x2 Move(const T &dx, const T &dy)
    {
        Matrix3x2 matrix = Matrix3x2::Identity();
        matrix._31 = dx;
        matrix._32 = dy;
        return matrix;
    }

    // 2d rotation transformation matrix
    static Matrix3x2 Rotate(const float &degree)
    {
        Matrix3x2 matrix;
        matrix._11 = matrix._22 = cos(degree);
        matrix._12 = sin(degree);
        matrix._21 = -sin(degree);
        return matrix;
    }

    // 2d scaling transformation matrix
    static Matrix3x2 Scale(const T &sx, const T &sy)
    {
        Matrix3x2 matrix;
        matrix._11 = sx;
        matrix._22 = sy;
        return matrix;
    }

    // 2d scaling transformation matrix
    static Matrix3x2 Scale(const T &s)
    {
        return Scale(s, s);
    }

    Matrix3x2()
    {
        _11 = _12 = _21 = _22 = _31 = _32 = (T)(0);
    }


    T _11;
    T _12;
    T _21;
    T _22;
    T _31;
    T _32;
};

typedef Matrix3x2<float> Matrix3x2F;