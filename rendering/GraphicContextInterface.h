#pragma once

#include "utils/Vec2d.h"
#include "utils/Matrix3x2.h"
#include "utils/Rect.h"
#include "utils/Vec2d.h"
#include "utils/ColorUtils.h"

#include <string>

class BitmapInterface;

// Abstract Rnder Target
class RenderTargetInterface
{
protected:
    friend class Chart; // Chart friendly only

    virtual ~RenderTargetInterface() { };

    virtual void Resize(unsigned int width, unsigned int height) = 0;

    virtual void BeginDraw() = 0;

    virtual void EndDraw() = 0;

    virtual void Clear(const WaaColour &color) = 0;

    virtual Matrix3x2F GetTransform() = 0;

    virtual Matrix3x2F GetTransformAndInvert() = 0;

    virtual void SetTransform(const Matrix3x2F &transform) = 0;

    virtual void DrawTexts(const wchar_t *text, const RectF &postion, const WaaColour &color, const float fontSize = 20.f) = 0;
    
    virtual void DrawLine(const Vec2dFloat &p0, const Vec2dFloat &p1, const WaaColour &color, const float lineWidth = 1.f) = 0;

    virtual void FillRound(const Vec2dFloat &p0, const WaaColour &color, const float radius = 2.f) = 0;

    virtual void DrawRound(const Vec2dFloat &p0, const WaaColour &color, const float radius = 3.f, const float strokeSize = 1.f) = 0;

    virtual void FillRoundedRectangle(const RectF &rect, const WaaColour &color, const float radius = 5.f) = 0;

    virtual void FillRectangle(const RectF &rect, const WaaColour &color /*, StrokeStyle */) = 0;

    virtual void DrawRectangle(const RectF &rect, const WaaColour &color, const float strokeSize /*, StrokeStyle */) = 0;

    virtual void DrawBitmap(BitmapInterface *pBitmat, const RectF &position) = 0;
};

class TextFormatInterface 
{

};

class BitmapInterface
{
public:
    virtual Vec2dFloat GetSize() = 0;
    virtual ~BitmapInterface() { }
};

class GraphicContextInterface
{
public:
    virtual ~GraphicContextInterface(){};

    virtual void GetDesktopDpi(Vec2dFloat &dpi) = 0;

    virtual RenderTargetInterface *CreateRenderTarget(const void *param) = 0;

    virtual TextFormatInterface *CreateTextFormat(float fontSize, wchar_t *fontType) = 0;

    virtual BitmapInterface * LoadDiskBitmapByResources(RenderTargetInterface *pRenderTarget, const wchar_t *fileName) = 0;
};