#pragma once

#include "ProtocolRequestListener.h"
#include "Transformation2D.h"

#include "settings/ChartSettings.h"
#include "utils/Matrix3x2.h"
#include "utils/Vec2d.h"
#include "utils/Rect.h"
#include "window/WindowListener.h"
#include "window/WindowInterface.h"

#include <string>
#include <vector>
#include <memory>
#include "Toolbar.h"


class RenderTargetInterface;
class BitmapInterface;

class Chart : public WindowListener, public ProtocolRequestListener
{
public:
    Chart(WindowInterface *pWindow);

    Chart(const Chart &b) = delete;

    virtual ~Chart();

    enum class MainChartType : unsigned char
    {
        Cartesian = 0x01,
        Pie = 0xE0,
    };


    unsigned char GetIndex() const;

    WindowInterface *Window() { return m_pWindow; }



protected:
    virtual void UpdateTransformation(Transformation2D &curr, const Transformation2D& target);

    virtual void DrawChartBackground();

    virtual void DrawAxisBackground();

    virtual void DrawDataContent();

    virtual void DrawChartToolIcons();

public:
    virtual void OnRender() override;

    virtual void OnDestroy() override;

    virtual void OnResize(unsigned int width, unsigned int height) override;

    virtual void OnMouseLButtonDown(unsigned int width, unsigned int height) override;

    virtual void OnMouseLButtonUp(unsigned int width, unsigned int height) override;

    virtual void OnMouseMove(const Vec2dFloat &mousePos, const bool lKeyDown = false, const bool mKeyDown = false, const bool rkeyDown = false, const bool ctrlKeyDown = false, const bool shiftKeyDown = false) override;

    virtual void OnKeyDown(short vkey) override;

    virtual void OnWheel(const Vec2dFloat mousePos, short delta) override;

    virtual void OnDataComming(/*const*/ std::unique_ptr<Dataset> &newData) override;

    void ClearAllDataset();

    bool OnSetShowData(unsigned int dataId, unsigned char shape) const;

private:

    bool IsPosInRect(const Vec2dFloat &mousePos, const RectF &rect);;

    void SetSuitableViewPort();

private:
    // Chart Index Number (ID number)
    unsigned char m_chartIndex;

    MainChartType m_ChartType;

    WindowInterface *m_pWindow;

    std::wstring m_title;

    std::vector<std::unique_ptr<Dataset> > m_DatasetList;

    Vec2dUint m_windowSize;

    Vec2dFloat m_mousePos;

    RectF m_contentRect;

    Transformation2D m_transCurr, m_transTarget;

    RenderTargetInterface *m_pRenderTarget;

    Toolbar m_Toobar;
    


    enum class MouseDragMode : unsigned char {
        PanInContent = 0x01,
        PanInX = 0x02,
        PanInY = 0x03,
        BoxZoomInContent = 0x11,
        BoxZoomInX = 0x12,
        BoxZoomInY = 0x13,
        BoxSelect
    };

    bool IsEquare(MouseDragMode mouseDragMode, Toolbar::Item::Enum toolbarIcon) {
        switch (mouseDragMode)
        {
        case Chart::MouseDragMode::PanInContent:
        case Chart::MouseDragMode::PanInX:
        case Chart::MouseDragMode::PanInY:
            if (toolbarIcon == Toolbar::Item::Enum::Pan) return true; else return false;
            break;
        case Chart::MouseDragMode::BoxZoomInContent:
        case Chart::MouseDragMode::BoxZoomInX:
        case Chart::MouseDragMode::BoxZoomInY:
            if (toolbarIcon == Toolbar::Item::Enum::BoxZoom) return true; else return false;
            break;
        case Chart::MouseDragMode::BoxSelect:
            if (toolbarIcon == Toolbar::Item::Enum::BoxSelect) return true; else return false;
            break;
        default:
            break;
        }

        return false;
    }

    MouseDragMode m_MouseDragMode;

    bool m_isDragStart;
    Vec2dFloat m_DragPosStart;

};