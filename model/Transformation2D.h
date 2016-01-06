#pragma once

#include "utils/Vec2d.h"
#include "utils/System.h"

struct Transformation2D
{
    Transformation2D() :
#ifdef TARGET_WINDOWS
        m_scale(1.f, -1.f),
#else
        m_scale(1.f, 1.f),
#endif // TARGET_WINDOWS
        m_rotate(0.f),
        m_move(0.f, 0.f)
    {
    }

    inline Matrix3x2F GetTransMatrix() {
        // Order is matter
        return Matrix3x2F::Scale(m_scale.x, m_scale.y) * Matrix3x2F::Rotate(m_rotate) * Matrix3x2F::Move(m_move.x, m_move.y);
    }

    Vec2dFloat m_scale, m_move;
    float m_rotate;
};