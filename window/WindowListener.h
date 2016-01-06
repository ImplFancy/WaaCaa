#pragma once

#include "utils/Vec2d.h"

class WindowListener
{
public:
    virtual void OnRender() = 0;

    virtual void OnDestroy() = 0;
    virtual void OnResize(unsigned int width, unsigned int height) = 0;
    virtual void OnMouseLButtonDown(unsigned int width, unsigned int height) = 0;
    virtual void OnMouseLButtonUp(unsigned int width, unsigned int height) = 0;
    virtual void OnMouseMove(const Vec2dFloat &mousePos, const bool lKeyDown = false, const bool mKeyDown = false, const bool rkeyDown = false, const bool ctrlKeyDown = false, const bool shiftKeyDown = false) = 0;
    virtual void OnKeyDown(short vkey) = 0;
    virtual void OnWheel(const Vec2dFloat mousePos, short delta) = 0;
};