#pragma once

#include "GraphicContextInterface.h"

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#include <vector>

// define SafeRelease
template<class Interface> inline void
SafeRelease(Interface **ppInterfaceToRelease) {
    if (*ppInterfaceToRelease != nullptr) {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}

class TextFormatInterfaceDirect2d : public TextFormatInterface
{
public:
    TextFormatInterfaceDirect2d();

    ~TextFormatInterfaceDirect2d();

    IDWriteTextFormat *pTextFormat;
};



class RenderTargetDirect2d : public RenderTargetInterface
{
public:
    RenderTargetDirect2d(ID2D1HwndRenderTarget *pRenderTarget);

    ~RenderTargetDirect2d();

    virtual void Resize(unsigned int width, unsigned int height) override;

    virtual void BeginDraw() override;

    virtual void EndDraw() override;

    virtual void Clear(const WaaColour &color) override;

    virtual Matrix3x2F GetTransform() override;

    virtual Matrix3x2F GetTransformAndInvert() override;

    virtual void SetTransform(const Matrix3x2F &transform) override;

    virtual void DrawTexts(const wchar_t *text, const RectF &postion, const WaaColour &color, const float fontSize = 20.f) override;

    virtual void DrawLine(const Vec2dFloat &p0, const Vec2dFloat &p1, const WaaColour &color, const float lineWidth = 1.f) override;
    
    virtual void FillRound(const Vec2dFloat &p0, const WaaColour &color, const float radius = 2.f) override;

    virtual void DrawRound(const Vec2dFloat &p0, const WaaColour &color, const float radius = 3.f, const float strokeSize = 1.f) override;

    virtual void FillRoundedRectangle(const RectF &rect, const WaaColour &color, const float radius = 5.f) override;

    virtual void DrawBitmap(BitmapInterface *pBitmat, const RectF &position) override;

    virtual void FillRectangle(const RectF &rect, const WaaColour &color) override;

    virtual void DrawRectangle(const RectF &rect, const WaaColour &color, const float strokeSize) override;

private:
    

    class SolidBrushBank
    {
    public:
        SolidBrushBank(ID2D1HwndRenderTarget * target);

        ~SolidBrushBank();

        ID2D1SolidColorBrush* Get(const WaaColour &query);

    private:
        ID2D1HwndRenderTarget *m_target;

        ID2D1SolidColorBrush *m_pBrush;
    };

    friend class GraphicContextDirect2d;
    ID2D1HwndRenderTarget *m_pRenderTarget;
    SolidBrushBank m_SolidBrushBank;
};

class BitmapDidrect2D : BitmapInterface
{
public:
    virtual Vec2dFloat GetSize() override;
    BitmapDidrect2D() :
        m_pBitmap(nullptr) 
    {

    }
    ~BitmapDidrect2D();
private:
    friend class GraphicContextDirect2d;
    friend class RenderTargetDirect2d;
    ID2D1Bitmap *m_pBitmap;
};



class GraphicContextDirect2d : public GraphicContextInterface
{
public:
    GraphicContextDirect2d();

    ~GraphicContextDirect2d();

    virtual void GetDesktopDpi(Vec2dFloat &dpi) override;

    virtual RenderTargetInterface *CreateRenderTarget(const void *param) override;

    virtual TextFormatInterface * CreateTextFormat(float fontSize, wchar_t *fontType) override;

    virtual BitmapInterface * LoadDiskBitmapByResources(RenderTargetInterface *pRenderTarget, const wchar_t *fileName) override;

private:

    HRESULT LoadResourceBitmap(
        ID2D1RenderTarget *pRenderTarget,
        IWICImagingFactory *pIWICFactory,
        PCWSTR resourceName,
        PCWSTR resourceType,
        UINT destinationWidth,
        UINT destinationHeight,
        ID2D1Bitmap **ppBitmap
        );

private:
    ID2D1Factory *m_pD2DFactory;
    IDWriteFactory *m_pDWriteFactory;
    IWICImagingFactory *m_pWICFactory;
};