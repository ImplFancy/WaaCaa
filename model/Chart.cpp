#include "Chart.h"

#include "window/WindowService.h"
#include "window/WindowInterface.h"
#include "utils/Log.h"
#include "utils/StringUtils.h"
#include "model/GradationsUtils.h"
#include "model/Dataset.h"


Chart::Chart(WindowInterface *pWindow) :
    m_chartIndex(0),
    m_pWindow(pWindow),
    m_pRenderTarget(nullptr),
    m_ChartType(MainChartType::Cartesian),
    m_MouseDragMode(MouseDragMode::PanInContent)
{
    static unsigned char s_chartIndex(1);

    m_DatasetList.clear();

    // set a new index number
    m_chartIndex = s_chartIndex++;
}

Chart::~Chart()
{
    if (m_pRenderTarget != nullptr) delete m_pRenderTarget;

    m_DatasetList.clear(); // unique_ptr
}

unsigned char Chart::GetIndex() const
{
    return m_chartIndex;
}

void Chart::UpdateTransformation(Transformation2D &curr, const Transformation2D& target)
{
    if (ChartSettings::Get().useEasingMode()) { 
        // simple easing mode
        float factor = .2f;
        curr.m_move += (target.m_move - curr.m_move) * factor;
        curr.m_scale += (target.m_scale - curr.m_scale) * factor;
        curr.m_rotate += (target.m_rotate - curr.m_rotate) * factor;
    }
    else {// no easing, direct mode
        curr = target;
    }
}

void Chart::DrawChartBackground()
{
    // Titile
    {
        m_pRenderTarget->DrawTexts(m_title.c_str(), RectF(10.f, 300.f, m_windowSize.x, 300.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor);
    }

    // Mouse Info, Pixel
    {
        wchar_t *out = L"Mouse Pos (px):";

        m_pRenderTarget->DrawTexts(
            out,
            //RectF(0.f, 15.f, 0.f, 80.f),
            RectF(m_mousePos.y, m_mousePos.y + 15.f, m_mousePos.x, m_mousePos.x + 80.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );

        out = StringUtils::FromFloat(m_mousePos.x).GetContent();
        m_pRenderTarget->DrawTexts(
            out,
            //RectF(0.f, 15.f, 70.f, 120.f),
            RectF(m_mousePos.y, m_mousePos.y + 15.f, m_mousePos.x + 70.f, m_mousePos.x + 120.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );


        out = StringUtils::FromFloat(m_mousePos.y).GetContent();

        m_pRenderTarget->DrawTexts(
            out,
            //RectF(0.f, 15.f, 120.f, 170.f),
            RectF(m_mousePos.y, m_mousePos.y + 15.f, m_mousePos.x + 120.f, m_mousePos.x + 170.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );
    }

    // Mouse Info, Real world
    {
        m_pRenderTarget->SetTransform(m_transCurr.GetTransMatrix());
        auto invertMatrix = m_pRenderTarget->GetTransformAndInvert();
        auto mousePos = m_mousePos * invertMatrix;
        m_pRenderTarget->SetTransform(Matrix3x2F::Identity());

        wchar_t *out = L"Mouse Pos (r):";

        m_pRenderTarget->DrawTexts(
            out,
            RectF(m_mousePos.y + 15.f, m_mousePos.y + 30.f, m_mousePos.x, m_mousePos.x + 80.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );

        out = StringUtils::FromFloat(mousePos.x).GetContent();
        m_pRenderTarget->DrawTexts(
            out,
            RectF(m_mousePos.y + 15.f, m_mousePos.y + 30.f, m_mousePos.x + 70.f, m_mousePos.x + 120.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );


        out = StringUtils::FromFloat(mousePos.y).GetContent();

        m_pRenderTarget->DrawTexts(
            out,
            RectF(m_mousePos.y + 15.f, m_mousePos.y + 30.f, m_mousePos.x + 120.f, m_mousePos.x + 170.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );

        static float i(0);
        out = StringUtils::FromFloat(i++).GetContent();

        m_pRenderTarget->DrawTexts(
            out,
            RectF(m_mousePos.y + 15.f, m_mousePos.y + 30.f, m_mousePos.x + 170.f, m_mousePos.x + 280.f),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor,
            10.f
            );
    }

    // Mouse Zoom Box
    {
        if (m_isDragStart) {
            if (m_MouseDragMode == MouseDragMode::BoxZoomInContent) {
                m_pRenderTarget->DrawRectangle(RectF(m_DragPosStart.y, m_mousePos.y, m_DragPosStart.x, m_mousePos.x), ColorTheme::GetDefaultTheme().m_BackgroundGray, 1.f);
            }
            else if (m_MouseDragMode == MouseDragMode::BoxZoomInX) {
                m_pRenderTarget->FillRectangle(RectF(m_contentRect.top, m_contentRect.bottom, m_DragPosStart.x, m_mousePos.x), ColorTheme::GetDefaultTheme().m_BackgroundGray.SetAlpha(0.4));
            }
            else if (m_MouseDragMode == MouseDragMode::BoxZoomInY) {
                m_pRenderTarget->FillRectangle(RectF(m_DragPosStart.y, m_mousePos.y, m_contentRect.left, m_contentRect.right), ColorTheme::GetDefaultTheme().m_BackgroundGray.SetAlpha(0.4));
            }
        }
    }
}

void Chart::DrawAxisBackground()
{
    if (m_pRenderTarget == nullptr) return;
    
    if (m_ChartType == MainChartType::Cartesian) {
        // rendering area, top
        m_pRenderTarget->DrawLine(
            Vec2dFloat(m_contentRect.left, m_contentRect.top),
            Vec2dFloat(m_contentRect.right, m_contentRect.top),
            ColorTheme::GetDefaultTheme().m_BackgroundGray
            );
        // rendering area, bottom
        m_pRenderTarget->DrawLine(
            Vec2dFloat(m_contentRect.left, m_contentRect.bottom),
            Vec2dFloat(m_contentRect.right, m_contentRect.bottom),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor
            );
        // rendering area, left
        m_pRenderTarget->DrawLine(
            Vec2dFloat(m_contentRect.left, m_contentRect.top),
            Vec2dFloat(m_contentRect.left, m_contentRect.bottom),
            ColorTheme::GetDefaultTheme().m_DefaultPenColor
            );
        // rendering area, right
        m_pRenderTarget->DrawLine(
            Vec2dFloat(m_contentRect.right, m_contentRect.top),
            Vec2dFloat(m_contentRect.right, m_contentRect.bottom),
            ColorTheme::GetDefaultTheme().m_BackgroundGray
            );


        // gradations lines

        auto coordinate = m_transCurr.m_move/* + m_coordinate_DragOffset*/;
        {
            auto absZoomFactor = fabs(m_transCurr.m_scale.x);
            auto absStep = Gradations::calcAbsStep(absZoomFactor);
            auto absStepPixel = Gradations::calcAbsStepPixel(absZoomFactor);
            auto minPixel = Gradations::calcStartPixel(m_contentRect.left - coordinate.x, absZoomFactor) + coordinate.x;
            auto maxPixel = m_contentRect.right;
            if (minPixel < maxPixel) {
                for (
                    auto val = minPixel;
                    val <= maxPixel;
                    val += absStepPixel) {
                    // draw number
                    wchar_t *out = StringUtils::FromFloat((val - coordinate.x) / absZoomFactor).GetContent();
                    m_pRenderTarget->DrawTexts(
                        out,
                        RectF(m_contentRect.bottom + 5, m_contentRect.bottom + 15, val - 10, val + 15),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor,
                        10.f
                        );
                    // raw gradations
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(val, m_contentRect.bottom + 5),
                        Vec2dFloat(val, m_contentRect.bottom - 2),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor
                        );
                    // raw gradations, background
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(val, m_contentRect.top),
                        Vec2dFloat(val, m_contentRect.bottom - 2),
                        ColorTheme::GetDefaultTheme().m_BackgroundGray
                        );
                    // fine gradations
                    for (auto littleVal = val + absStepPixel / 5.f;
                    (littleVal < maxPixel) && (littleVal < val + absStepPixel);
                        littleVal += absStepPixel / 5.f)
                        m_pRenderTarget->DrawLine(
                            Vec2dFloat(littleVal, m_contentRect.bottom + 3),
                            Vec2dFloat(littleVal, m_contentRect.bottom - 0),
                            ColorTheme::GetDefaultTheme().m_DefaultPenColor
                            );
                }
                // fine gradations, before startPoint
                for (auto littleVal = minPixel;
                littleVal > m_contentRect.left;
                    littleVal -= absStepPixel / 5.f)
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(littleVal, m_contentRect.bottom + 3),
                        Vec2dFloat(littleVal, m_contentRect.bottom - 0),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor
                        );
            }
            else
            {
                //assert(false);
            }
        }
        {
            auto absZoomFactor = fabs(m_transCurr.m_scale.y);
            auto absStep = Gradations::calcAbsStep(absZoomFactor);
            auto absStepPixel = Gradations::calcAbsStepPixel(absZoomFactor);
            auto minPixel = Gradations::calcStartPixel(m_contentRect.top - coordinate.y, absZoomFactor) + coordinate.y;
            auto maxPixel = m_contentRect.bottom;
            if (minPixel < maxPixel) {
                for (
                    auto val = minPixel;
                    val <= maxPixel;
                    val += absStepPixel) {
                    // draw number
                    wchar_t *out = StringUtils::FromFloat((val - coordinate.y) / -absZoomFactor).GetContent();
                    m_pRenderTarget->DrawTexts(
                        out,
                        RectF(val - 7.5, val + 15, m_contentRect.left - 30, m_contentRect.left - 5),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor,
                        10.f
                        );
                    // raw gradations
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(m_contentRect.left - 5, val),
                        Vec2dFloat(m_contentRect.left + 2, val),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor
                        );
                    // raw gradations, background
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(m_contentRect.left + 2, val),
                        Vec2dFloat(m_contentRect.right, val),
                        ColorTheme::GetDefaultTheme().m_BackgroundGray
                        );
                    // fine gradations
                    for (auto littleVal = val + absStepPixel / 5.f;
                    (littleVal < maxPixel) && (littleVal < val + absStepPixel);
                        littleVal += absStepPixel / 5.f)
                        m_pRenderTarget->DrawLine(
                            Vec2dFloat(m_contentRect.left - 3, littleVal),
                            Vec2dFloat(m_contentRect.left - 0, littleVal),
                            ColorTheme::GetDefaultTheme().m_DefaultPenColor
                            );
                }
                // fine gradations, before startPoint
                for (auto littleVal = minPixel;
                littleVal > m_contentRect.top;
                    littleVal -= absStepPixel / 5.f)
                    m_pRenderTarget->DrawLine(
                        Vec2dFloat(m_contentRect.left - 3, littleVal),
                        Vec2dFloat(m_contentRect.left - 0, littleVal),
                        ColorTheme::GetDefaultTheme().m_DefaultPenColor
                        );
            }
            else
            {
                //assert(false);
            }
        }
    }
    else if (m_ChartType == MainChartType::Pie) {
        // TODO
    }
    else {
        // Unkonw Chart Type
    }
}

void Chart::DrawDataContent()
{
    auto needToDrawFunc = [&](const Vec2dFloat point, const RectF contentRect) -> bool {
        if (point.x != point.x) return false; // NaN
        if (point.y != point.y) return false; // NaN
        return !(point.x < contentRect.left || point.x > contentRect.right || point.y < contentRect.top || point.y > contentRect.bottom);
    };

    auto drawCircle = [&](const Vec2dFloat &pointOrig, WaaColour color) {
        auto point = pointOrig * m_transCurr.GetTransMatrix();
        if (needToDrawFunc(point, m_contentRect)) {
            m_pRenderTarget->FillRound(point, color.SetAlpha(0.8));
            m_pRenderTarget->DrawRound(point, color);
        }
    };

    auto draw1DLine = [&](void *dataToDraw, unsigned int dataLen, Dataset::ElemDataType elemDataType) {
        for (auto index = 0u; index < dataLen - 1; index++) {
            Vec2dFloat point;
            float yValue, yValueNext;

            switch (elemDataType)
            {
            case Dataset::ElemDataType::_ElemInt16: {
                short *pInt16 = (short *)dataToDraw;
                yValue = pInt16[index];
                yValueNext = pInt16[index + 1];
                break;
            }
            case Dataset::ElemDataType::_ElemUInt16: {
                unsigned short *pUInt16 = (unsigned short *)dataToDraw;
                yValue = pUInt16[index];
                yValueNext = pUInt16[index + 1];
                break;
            }
            case Dataset::ElemDataType::_ElemInt32: {
                int *pInt32 = (int *)dataToDraw;
                yValue = pInt32[index];
                yValueNext = pInt32[index + 1];
                break;
            }
            case Dataset::ElemDataType::_ElemUInt32: {
                unsigned int *pUInt32 = (unsigned int *)dataToDraw;
                yValue = pUInt32[index];
                yValueNext = pUInt32[index + 1];
                break;
            }
            case Dataset::ElemDataType::_ElemFloat32: {
                float *pFloat32 = (float *)dataToDraw;
                yValue = pFloat32[index];
                yValueNext = pFloat32[index + 1];
                break;
            }
            case Dataset::ElemDataType::_ElemFloat64: {
                double *pFloat64 = (double *)dataToDraw;
                yValue = pFloat64[index];
                yValueNext = pFloat64[index + 1];
                break;
            }
            default:
                break;
            }
        }
    };

    auto corlorIndex = 0u;
    for (auto &dataset : m_DatasetList) {
        auto currColor = ColorTheme::GetDefaultTheme().m_DataContentList[corlorIndex++];
        dataset->BeginRetrival();
        for ( ; ; ) {
            auto pP = dataset->GetNextPoint();
            if (pP == nullptr) break;
            drawCircle(*pP, currColor);
        }
    }
}

void Chart::DrawChartToolIcons()
{
    if (m_pRenderTarget == nullptr) return;
    
    if (!m_Toobar.IsInited()) {
        m_Toobar.AddIconStart();
        m_Toobar.AddIcon(L"Reset", WindowService::Instance().GraphicContext()->LoadDiskBitmapByResources(m_pRenderTarget, L"IDI_reset"), Toolbar::Item::Enum::ResetView);
        m_Toobar.AddSpace();
        m_Toobar.AddIcon(L"Box Select", WindowService::Instance().GraphicContext()->LoadDiskBitmapByResources(m_pRenderTarget, L"IDI_boxSelect"), Toolbar::Item::Enum::BoxSelect);
        m_Toobar.AddIcon(L"Box Zoom", WindowService::Instance().GraphicContext()->LoadDiskBitmapByResources(m_pRenderTarget, L"IDI_boxZoom"), Toolbar::Item::Enum::BoxZoom);
        m_Toobar.AddIcon(L"Pan", WindowService::Instance().GraphicContext()->LoadDiskBitmapByResources(m_pRenderTarget, L"IDI_pan"), Toolbar::Item::Enum::Pan);
    }

    for (auto &icon : m_Toobar.m_Items) {
        auto theRect = icon->GetPosOffset(m_contentRect.top / 4.f, m_contentRect.right);

        if (IsEquare(m_MouseDragMode, icon->m_type)) {
            m_pRenderTarget->DrawRectangle(theRect, ColorTheme::GetDefaultTheme().m_Warning, 1.f);
        }
        else {
            m_pRenderTarget->DrawRectangle(theRect, ColorTheme::GetDefaultTheme().m_BackgroundGray.SetAlpha(0.8), 1.f);
        }

        m_pRenderTarget->DrawBitmap(icon->m_pBitmap, theRect.Scale(0.78));
    }
}

void Chart::OnRender()
{
    if (m_pRenderTarget == nullptr) {
        m_pRenderTarget = WindowService::Instance().GraphicContext()->CreateRenderTarget(m_pWindow->m_pHandler);
    }

    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->Clear(ColorTheme::GetDefaultTheme().m_ClearColor);

    if (m_windowSize.x > ChartSettings::Get().GetMinWindowWidth() &&
        m_windowSize.y > ChartSettings::Get().GetMinWindowHeight())
    {
        UpdateTransformation(m_transCurr, m_transTarget);

        DrawAxisBackground();
        DrawDataContent();
        DrawChartBackground();
        DrawChartToolIcons();

    }
    else {
        m_pRenderTarget->DrawTexts(
            L"Window Size Is Too Small !!!",
            RectF(0, m_windowSize.y, 0, m_windowSize.x), ColorTheme::GetDefaultTheme().m_Warning);
    }
    
    m_pRenderTarget->EndDraw();
}

void Chart::OnDestroy()
{
    WindowService::Instance().CloseWindow(m_chartIndex);
}

void Chart::OnResize(unsigned int width, unsigned int height)
{
    Vec2dUint newWindowSize(width, height);
    RectF newContentRect = { 40.f, (float)(newWindowSize.y - 30), 35.f, (float)(newWindowSize.x - 15) };
    m_windowSize = newWindowSize;
    m_contentRect = newContentRect;

    if (m_pRenderTarget != nullptr) {
        m_pRenderTarget->Resize(width, height);
    }

    SetSuitableViewPort();
}

void Chart::OnMouseLButtonDown(unsigned int width, unsigned int height)
{
    auto mousePos = m_mousePos;

    bool isClickIcon = false;
    for (auto &icon : m_Toobar.m_Items) {
        auto iconRect = icon->GetPosOffset(m_contentRect.top / 4.f, m_contentRect.right);
        if (IsPosInRect(mousePos, iconRect)) {
            if (icon->m_type == Toolbar::Item::Enum::ResetView) {
                SetSuitableViewPort();
            }
            else if (icon->m_type == Toolbar::Item::Enum::Pan) {
                m_MouseDragMode = MouseDragMode::PanInContent;
            }
            else if (icon->m_type == Toolbar::Item::Enum::BoxZoom) {
                m_MouseDragMode = MouseDragMode::BoxZoomInContent;
            }
            else if (icon->m_type == Toolbar::Item::Enum::BoxSelect) {
                m_MouseDragMode = MouseDragMode::BoxSelect;
            }
            else {
                continue;
            }

            isClickIcon = true;
        }
    }

    if (!isClickIcon) {
        if (IsPosInRect(mousePos, m_contentRect)) {
            if (m_MouseDragMode == MouseDragMode::PanInContent || m_MouseDragMode == MouseDragMode::PanInX || m_MouseDragMode == MouseDragMode::PanInY) {
                m_MouseDragMode = MouseDragMode::PanInContent;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }
            else if (m_MouseDragMode == MouseDragMode::BoxZoomInContent || m_MouseDragMode == MouseDragMode::BoxZoomInX || m_MouseDragMode == MouseDragMode::BoxZoomInY) {
                m_MouseDragMode = MouseDragMode::BoxZoomInContent;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }
        }
        else if (mousePos.y > m_contentRect.bottom) {
            if (m_MouseDragMode == MouseDragMode::PanInContent || m_MouseDragMode == MouseDragMode::PanInX || m_MouseDragMode == MouseDragMode::PanInY) {
                m_MouseDragMode = MouseDragMode::PanInX;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }
            else if (m_MouseDragMode == MouseDragMode::BoxZoomInContent || m_MouseDragMode == MouseDragMode::BoxZoomInX || m_MouseDragMode == MouseDragMode::BoxZoomInY) {
                m_MouseDragMode = MouseDragMode::BoxZoomInX;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }

        }
        else if (mousePos.x < m_contentRect.left) {
            if (m_MouseDragMode == MouseDragMode::PanInContent || m_MouseDragMode == MouseDragMode::PanInX || m_MouseDragMode == MouseDragMode::PanInY) {
                m_MouseDragMode = MouseDragMode::PanInY;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }
            else if (m_MouseDragMode == MouseDragMode::BoxZoomInContent || m_MouseDragMode == MouseDragMode::BoxZoomInX || m_MouseDragMode == MouseDragMode::BoxZoomInY) {
                m_MouseDragMode = MouseDragMode::BoxZoomInY;
                m_isDragStart = true;
                m_DragPosStart = mousePos;
            }
        }
    }
    m_mousePos = mousePos;
}

void Chart::OnMouseLButtonUp(unsigned int width, unsigned int height)
{
    auto mousePos = m_mousePos;
    if (!m_isDragStart) return;

    if (m_MouseDragMode == Chart::MouseDragMode::BoxZoomInContent) {
        auto zoomSize = mousePos - m_DragPosStart;
        // do zoom in
        if (fabs(zoomSize.x) > ChartSettings::Get().GetMinZoomWidth() &&
            fabs(zoomSize.y) > ChartSettings::Get().GetMinZoomHeight()) {
            m_transTarget.m_scale.x *= m_contentRect.Width() / (fabs(zoomSize.x));
            m_transTarget.m_scale.y *= m_contentRect.Height() / (fabs(zoomSize.y));

            Vec2dFloat thePoint = {
                std::fmin(mousePos.x, m_DragPosStart.x),
                std::fmin(mousePos.y, m_DragPosStart.y)
            };

            m_pRenderTarget->SetTransform(m_transCurr.GetTransMatrix());
            m_transTarget.m_move = thePoint * m_pRenderTarget->GetTransformAndInvert();
            m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
            m_transTarget.m_move = m_transTarget.m_move.dotMultiply(m_transTarget.m_scale * -1);
            //m_transTarget.m_move.y *= -m_transTarget.m_scale.y;
            m_transTarget.m_move += Vec2dFloat(m_contentRect.left, m_contentRect.top);

        }
    }
    else if (m_MouseDragMode == Chart::MouseDragMode::BoxZoomInX) {
        auto zoomSize = mousePos - m_DragPosStart;
        // do zoom in
        if (fabs(zoomSize.x) > ChartSettings::Get().GetMinZoomWidth()) {
            m_transTarget.m_scale.x *= m_contentRect.Width() / (fabs(zoomSize.x));

            Vec2dFloat thePoint = {
                std::fmin(mousePos.x, m_DragPosStart.x),
                m_contentRect.top
            };

            m_pRenderTarget->SetTransform(m_transCurr.GetTransMatrix());
            m_transTarget.m_move = thePoint * m_pRenderTarget->GetTransformAndInvert();
            m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
            m_transTarget.m_move = m_transTarget.m_move.dotMultiply(m_transTarget.m_scale * -1);
            m_transTarget.m_move += Vec2dFloat(m_contentRect.left, m_contentRect.top);

        }
    }
    else if (m_MouseDragMode == Chart::MouseDragMode::BoxZoomInY) {
        auto zoomSize = mousePos - m_DragPosStart;
        // do zoom in
        if (fabs(zoomSize.y) > ChartSettings::Get().GetMinZoomHeight()) {
            m_transTarget.m_scale.y *= m_contentRect.Height() / (fabs(zoomSize.y));

            Vec2dFloat thePoint = {
                m_contentRect.left,
                std::fmin(mousePos.y, m_DragPosStart.y)
            };

            m_pRenderTarget->SetTransform(m_transCurr.GetTransMatrix());
            m_transTarget.m_move = thePoint * m_pRenderTarget->GetTransformAndInvert();
            m_pRenderTarget->SetTransform(Matrix3x2F::Identity());
            m_transTarget.m_move = m_transTarget.m_move.dotMultiply(m_transTarget.m_scale * -1);
            m_transTarget.m_move += Vec2dFloat(m_contentRect.left, m_contentRect.top);

        }
    }

    m_isDragStart = false;
    m_mousePos = mousePos;
}

void Chart::OnMouseMove(const Vec2dFloat &mousePos, const bool lKeyDown /*= false*/, const bool mKeyDown /*= false*/, const bool rkeyDown /*= false*/, const bool ctrlKeyDown /*= false*/, const bool shiftKeyDown /*= false*/)
{
    // TODO: SetCapture? for out side of window

    Vec2dFloat newPos = mousePos;
    auto diffPos = newPos - m_mousePos;

    if (lKeyDown) {
        if (m_isDragStart) {
                switch (m_MouseDragMode)
                {
                case Chart::MouseDragMode::PanInContent:
                    m_transTarget.m_move += diffPos;
                    break;
                case Chart::MouseDragMode::PanInX:
                    m_transTarget.m_move.x += diffPos.x;
                    break;
                case Chart::MouseDragMode::PanInY:
                    m_transTarget.m_move.y += diffPos.y;
                    break;
                case Chart::MouseDragMode::BoxZoomInContent:
                    break;
                case Chart::MouseDragMode::BoxSelect:
                    break;
                default:
                    break;
                }
        }
    }
    else {
        m_isDragStart = false;
    }

    m_mousePos = mousePos;
}

void Chart::OnKeyDown(short vkey)
{
}

void Chart::OnWheel(const Vec2dFloat mousePos, short delta)
{
    static const float sc_maxZoom = 15000.0f;
    static const float sc_minZoom = .0010f;
    static const float sc_zoomStep = 1.5f;

    auto factor = pow(sc_zoomStep, delta / 120.0f);

    if (factor >= sc_maxZoom) factor = sc_maxZoom;
    if (factor <= sc_minZoom) factor = sc_minZoom;

    if (IsPosInRect(mousePos, m_contentRect)) {
        auto targetZoomFactor = m_transTarget.m_scale * factor;
        auto minV = std::fabs(std::fmin(fabs(targetZoomFactor.x), fabs(targetZoomFactor.y)));
        auto maxV = std::fabs(std::fmax(fabs(targetZoomFactor.x), fabs(targetZoomFactor.y)));

        if (minV < sc_minZoom && delta < 0) return;
        if (maxV > sc_maxZoom && delta > 0) return;

        m_mousePos = mousePos;
        m_transTarget.m_scale = targetZoomFactor;

        // keep mouse pos
        m_transTarget.m_move = m_mousePos - (m_mousePos - m_transTarget.m_move) * factor;
    } 
    else if (mousePos.y > m_contentRect.bottom) {
        auto targetZoomFactor = m_transTarget.m_scale.x * factor;
        if (fabs(targetZoomFactor) < sc_minZoom && delta < 0) return;
        if (fabs(targetZoomFactor) > sc_maxZoom && delta > 0) return;

        m_mousePos = mousePos;
        m_transTarget.m_scale.x = targetZoomFactor;

        // keep x-axis of mouse pos
        m_transTarget.m_move.x = m_mousePos.x - (m_mousePos.x - m_transTarget.m_move.x) * factor;
    } 
    else if (mousePos.x < m_contentRect.left) {
        auto targetZoomFactor = m_transTarget.m_scale.y * factor;
        if (fabs(targetZoomFactor) < sc_minZoom && delta < 0) return;
        if (fabs(targetZoomFactor) > sc_maxZoom && delta > 0) return;

        m_mousePos = mousePos;
        m_transTarget.m_scale.y = targetZoomFactor;

        // keep y-axis of mouse pos
        m_transTarget.m_move.y = m_mousePos.y - (m_mousePos.y - m_transTarget.m_move.y) * factor;
    }
}

void Chart::OnDataComming(/*const*/ std::unique_ptr<Dataset> &newData)
{
    m_DatasetList.push_back(std::move(newData));

    SetSuitableViewPort();
}

bool Chart::IsPosInRect(const Vec2dFloat &mousePos, const RectF &rect)
{
    if (mousePos.x < rect.left) return false;
    if (mousePos.x > rect.right) return false;
    if (mousePos.y < rect.top) return false;
    if (mousePos.y > rect.bottom) return false;
    return true;
}

void Chart::SetSuitableViewPort()
{
    if (m_DatasetList.empty()) return;

    // TODO: considering if (m_ChartSubType == SubType::DrawOneWave) 

    float minY, minX, maxY, maxX;

    minY = minX = std::numeric_limits<float>::max();
    maxY = maxX = std::numeric_limits<float>::min();

    unsigned int nbDatapoint = 0u;

    for (auto &oneDataset : m_DatasetList) {
        auto currMinX = oneDataset->GetMinX();
        auto currMaxX = oneDataset->GetMaxX();
        auto currMinY = oneDataset->GetMinY();
        auto currMaxY = oneDataset->GetMaxY();


        if (currMinX < minX) { minX = currMinX; }
        if (currMaxX > maxX) { maxX = currMaxX; }
        if (currMinY < minY) { minY = currMinY; }
        if (currMaxY > maxY) { maxY = currMaxY; }

        nbDatapoint += oneDataset->GetNbOfDataPoint();
    }



    if (nbDatapoint == 1u) {
        m_transTarget.m_scale.x = 1.f;
        m_transTarget.m_scale.y = 1.f;
    } 
    else {
        m_transTarget.m_scale.x = m_contentRect.AbsWidth() / fabs(maxX - minX) * .9f;
        m_transTarget.m_scale.y = m_contentRect.AbsHeight() / fabs(maxY - minY) * .9f;
    }

    /*
    TODO: handle out-of-range of m_transTarget.m_scale
    */

    if (OS::s_Type == OS::Type::OS_WIN32) {
        m_transTarget.m_scale.y *= -1;
    }

    // left-bottom (math coordinate system)
    Vec2dFloat modelLbPoint = {
        (maxX + minX) / 2.f - (maxX - minX) / .9f / 2.f,
        (maxY + minY) / 2.f - (maxY - minY) / .9f / 2.f
    };

    // movement vector (screen coordinate system)
    m_transTarget.m_move = 
        Vec2dFloat(m_contentRect.left, m_contentRect.bottom) 
        - modelLbPoint.dotMultiply(m_transTarget.m_scale);
}