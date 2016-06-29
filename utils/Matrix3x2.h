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

	std::array<T, 2> operator*(const std::array<T, 2> &orig) const {
		std::array<T, 2> newVec;
		newVec.at(0) = orig.at(0) * _11 + orig.at(1) * _21 + _31;
		newVec.at(1) = orig.at(0) * _12 + orig.at(1) * _22 + _32;
		return newVec;
	}

	Matrix3x2 CalcRevert() const {
		if (Determinant() == 0) return *this;
		Matrix3x2 result;
		result._11 = _22 / Determinant();
		result._22 = _11 / Determinant();
		result._21 = _21 / Determinant();
		result._31 = (_21 * _32 - _31*_22) / Determinant();
		result._32 = -(_11*_32 - _31*_12) / Determinant();
		//result._33 = (_11*_22 - _12*_21) / Determinant();
		return result;
	}


private:
	T Determinant() const {
		return (_11 * _22 - _12 * _21);
	}
	
    T _11;
    T _12;
    T _21;
    T _22;
    T _31;
    T _32;
};

typedef Matrix3x2<float> Matrix3x2F;


// std::array operator-
template <class T>
T scaleWith(const T& a1, const T& a2)
{
	T a;
	for (typename T::size_type i = 0; i < a1.size(); i++)
		a[i] = a1[i] * a2[i];
	return a;
}
