#include "GraphicContextDirect2d.h"

#include "utils/Common.h"
#include "window/WindowService.h"
#include "window/WindowWin32.h"

#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


TextFormatInterfaceDirect2d::TextFormatInterfaceDirect2d() :
    pTextFormat(nullptr)
{

}

TextFormatInterfaceDirect2d::~TextFormatInterfaceDirect2d()
{
    SafeRelease(&pTextFormat);
}


RenderTargetDirect2d::RenderTargetDirect2d(ID2D1HwndRenderTarget *pRenderTarget) :
    m_pRenderTarget(pRenderTarget),
    m_SolidBrushBank(pRenderTarget)
{

}

RenderTargetDirect2d::~RenderTargetDirect2d()
{
    SafeRelease(&m_pRenderTarget); // DiscardDeviceResources
}

void RenderTargetDirect2d::Resize(unsigned int width, unsigned int height)
{
    if (m_pRenderTarget == nullptr) return;

    D2D1_SIZE_U size = { width, height };

    m_pRenderTarget->Resize(size);
}

void RenderTargetDirect2d::BeginDraw()
{
    if (m_pRenderTarget == nullptr) return;

    m_pRenderTarget->BeginDraw();
}

void RenderTargetDirect2d::EndDraw()
{
    if (m_pRenderTarget == nullptr) return;

    auto hr = m_pRenderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET) {
        SafeRelease(&m_pRenderTarget); // DiscardDeviceResources
    }
}

void RenderTargetDirect2d::Clear(const WaaColour &color)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->Clear(D2D1::ColorF(color.m_rgb, color.m_alpha));
}

Matrix3x2F RenderTargetDirect2d::GetTransform()
{
    Matrix3x2F matrix;
    D2D1::Matrix3x2F d2dMatrix;
    m_pRenderTarget->GetTransform(&d2dMatrix);

    matrix._11 = d2dMatrix._11;
    matrix._12 = d2dMatrix._12;
    matrix._21 = d2dMatrix._21;
    matrix._22 = d2dMatrix._22;
    matrix._31 = d2dMatrix._31;
    matrix._32 = d2dMatrix._32;

    return matrix;
}

Matrix3x2F RenderTargetDirect2d::GetTransformAndInvert()
{
    Matrix3x2F matrix;
    D2D1::Matrix3x2F d2dMatrix;
    m_pRenderTarget->GetTransform(&d2dMatrix);
    d2dMatrix.Invert();

    matrix._11 = d2dMatrix._11;
    matrix._12 = d2dMatrix._12;
    matrix._21 = d2dMatrix._21;
    matrix._22 = d2dMatrix._22;
    matrix._31 = d2dMatrix._31;
    matrix._32 = d2dMatrix._32;

    return matrix;
}

void RenderTargetDirect2d::SetTransform(const Matrix3x2F &transform)
{
    D2D1::Matrix3x2F d2d1matrix;
    d2d1matrix._11 = transform._11;
    d2d1matrix._12 = transform._12;
    d2d1matrix._21 = transform._21;
    d2d1matrix._22 = transform._22;
    d2d1matrix._31 = transform._31;
    d2d1matrix._32 = transform._32;

    m_pRenderTarget->SetTransform(d2d1matrix);
}

void RenderTargetDirect2d::DrawTexts(const wchar_t *text, const RectF &postion, const WaaColour &color /*= ColorEnum::Black*/, const float fontSize /*= 20.f*/)
{
    if (m_pRenderTarget == nullptr) return;

    if (text == nullptr) return;

    auto *pFormat = (TextFormatInterfaceDirect2d *)WindowService::Instance().GraphicContext()->CreateTextFormat(fontSize, L"Calibri");

    m_pRenderTarget->DrawText(text, wcslen(text), pFormat->pTextFormat,
        D2D1::RectF(postion.left, postion.top, postion.right, postion.bottom),
        m_SolidBrushBank.Get(color), D2D1_DRAW_TEXT_OPTIONS_NONE);

    delete pFormat;
}

void RenderTargetDirect2d::DrawLine(const Vec2dFloat &p0, const Vec2dFloat &p1, const WaaColour &color, const float lineWidth /*= 1.f*/)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->DrawLine(D2D1::Point2F(p0.x, p0.y), D2D1::Point2F(p1.x, p1.y),
        m_SolidBrushBank.Get(color), lineWidth);
 }

void RenderTargetDirect2d::FillRound(const Vec2dFloat &p0, const WaaColour &color, const float radius /*= 5.f*/)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(p0.x, p0.y), radius, radius), 
        m_SolidBrushBank.Get(color));
}

void RenderTargetDirect2d::DrawRound(const Vec2dFloat &p0, const WaaColour &color, const float radius/* = 5.f*/, const float strokeSize/* = 1.f*/)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(p0.x, p0.y), radius, radius), 
        m_SolidBrushBank.Get(color), strokeSize);
}

void RenderTargetDirect2d::FillRoundedRectangle(const RectF &rect, const WaaColour &color, const float radius)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->FillRoundedRectangle(
        D2D1::RoundedRect(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), radius, radius),
        m_SolidBrushBank.Get(color));
}

void RenderTargetDirect2d::DrawBitmap(BitmapInterface *pBitmat, const RectF &position)
{
    auto pBitmapD2D1 = (BitmapDidrect2D *)pBitmat;
    m_pRenderTarget->DrawBitmap(
        pBitmapD2D1->m_pBitmap,
        D2D1::RectF(position.left, position.top, position.right, position.bottom));
}

void RenderTargetDirect2d::FillRectangle(const RectF &rect, const WaaColour &color)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_SolidBrushBank.Get(color));
}

void RenderTargetDirect2d::DrawRectangle(const RectF &rect, const WaaColour &color, const float strokeSize)
{
    if (m_pRenderTarget == nullptr) return;
    m_pRenderTarget->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), m_SolidBrushBank.Get(color), strokeSize);
}

GraphicContextDirect2d::GraphicContextDirect2d() :
    m_pD2DFactory(nullptr),
    m_pDWriteFactory(nullptr),
    m_pWICFactory(nullptr)
{
    auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr)) {
        Log::e() << "D2D1CreateFactory Failed!";
        return;
    }

    // Create WIC factory.
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void **>(&m_pWICFactory)
        );
    if (FAILED(hr)) {
        Log::e() << "CoCreateInstance m_pWICFactory Failed!";
        return;
    }

    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_pDWriteFactory),
        reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
        );
}

GraphicContextDirect2d::~GraphicContextDirect2d()
{
}

void GraphicContextDirect2d::GetDesktopDpi(Vec2dFloat &dpi)
{
    m_pD2DFactory->GetDesktopDpi(&(dpi.x), &(dpi.y));
}

RenderTargetInterface *GraphicContextDirect2d::CreateRenderTarget(const void *param)
{
    RECT rc;
    HWND hWnd = *(HWND *)(param);
    GetClientRect(hWnd, &rc);

    // Create a Direct2D render target.
    ID2D1HwndRenderTarget *pRenderTarget;
    auto hr = m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWnd, 
            D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            )),
        &pRenderTarget
        );


    return new RenderTargetDirect2d(pRenderTarget);
}

TextFormatInterface * GraphicContextDirect2d::CreateTextFormat(float fontSize, wchar_t *fontType)
{
    TextFormatInterfaceDirect2d *d2dTextFormat = new TextFormatInterfaceDirect2d;

    auto hr = m_pDWriteFactory->CreateTextFormat(
        fontType,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        static_cast<FLOAT>(fontSize),
        L"en-US", //locale
        &(d2dTextFormat->pTextFormat)
        );

    return d2dTextFormat;
}

BitmapInterface * GraphicContextDirect2d::LoadDiskBitmapByResources(RenderTargetInterface *pRenderTarget, const wchar_t *resourceName)
{
    BitmapDidrect2D *pBitmap = new BitmapDidrect2D;

    auto pRT = (RenderTargetDirect2d *)pRenderTarget;
    auto hr = LoadResourceBitmap(
        pRT->m_pRenderTarget,
        m_pWICFactory,
        resourceName,
        L"Image",
        0,
        0,
        &(pBitmap->m_pBitmap)
        );

    if (FAILED(hr)) {
        Log::e() << "Load Image Failed!";
        delete pBitmap;
        return nullptr;
    }

    return pBitmap;
}

HRESULT GraphicContextDirect2d::LoadResourceBitmap(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap)
{
    HRESULT hr = S_OK;
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICFormatConverter *pConverter = NULL;
    IWICBitmapScaler *pScaler = NULL;

    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void *pImageFile = NULL;
    DWORD imageFileSize = 0;

    imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);

    hr = imageResHandle ? S_OK : E_FAIL;
    if (SUCCEEDED(hr)) {
        imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

        hr = imageResDataHandle ? S_OK : E_FAIL;
    }
    if (SUCCEEDED(hr)) {
        pImageFile = LockResource(imageResDataHandle);

        hr = pImageFile ? S_OK : E_FAIL;
    }
    if (SUCCEEDED(hr)) {
        imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

        hr = imageFileSize ? S_OK : E_FAIL;
    }
    if (SUCCEEDED(hr)) {
        hr = pIWICFactory->CreateStream(&pStream);
    }
    if (SUCCEEDED(hr)) {
        hr = pStream->InitializeFromMemory(
            reinterpret_cast<BYTE*>(pImageFile),
            imageFileSize
            );
    }
    if (SUCCEEDED(hr)) {
        hr = pIWICFactory->CreateDecoderFromStream(
            pStream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder
            );
    }
    if (SUCCEEDED(hr)) {
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr)) {
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }
    if (SUCCEEDED(hr)) {
        if (destinationWidth != 0 || destinationHeight != 0) {
            UINT originalWidth, originalHeight;
            hr = pSource->GetSize(&originalWidth, &originalHeight);
            if (SUCCEEDED(hr)) {
                if (destinationWidth == 0) {
                    FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                    destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                }
                else if (destinationHeight == 0) {
                    FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                    destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                }

                hr = pIWICFactory->CreateBitmapScaler(&pScaler);
                if (SUCCEEDED(hr)) {
                    hr = pScaler->Initialize(
                        pSource,
                        destinationWidth,
                        destinationHeight,
                        WICBitmapInterpolationModeCubic
                        );
                    if (SUCCEEDED(hr)) {
                        hr = pConverter->Initialize(
                            pScaler,
                            GUID_WICPixelFormat32bppPBGRA,
                            WICBitmapDitherTypeNone,
                            NULL,
                            0.f,
                            WICBitmapPaletteTypeMedianCut
                            );
                    }
                }
            }
        }
        else {
            hr = pConverter->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                NULL,
                0.f,
                WICBitmapPaletteTypeMedianCut
                );
        }
    }
    if (SUCCEEDED(hr)) {
        //create a Direct2D bitmap from the WIC bitmap.
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
            );
    }

    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

    return hr;
}

RenderTargetDirect2d::SolidBrushBank::SolidBrushBank(ID2D1HwndRenderTarget * target) :
    m_target(target), m_pBrush(nullptr)
{
}

RenderTargetDirect2d::SolidBrushBank::~SolidBrushBank()
{
    SafeRelease(&m_pBrush);
}

ID2D1SolidColorBrush* RenderTargetDirect2d::SolidBrushBank::Get(const WaaColour & color)
{
    if (m_pBrush == nullptr) {
        auto hr = m_target->CreateSolidColorBrush(
            D2D1::ColorF(color.m_rgb, color.m_alpha),
            &m_pBrush
            );

        if (FAILED(hr)) return nullptr;
    } 

    m_pBrush->SetColor(D2D1::ColorF(color.m_rgb, color.m_alpha));
    return m_pBrush;
}

Vec2dFloat BitmapDidrect2D::GetSize()
{
    auto size = m_pBitmap->GetSize();
    return Vec2dFloat(size.width, size.height);
}

BitmapDidrect2D::~BitmapDidrect2D()
{
    if (m_pBitmap != nullptr) {
        SafeRelease(&m_pBitmap);
    }
}
