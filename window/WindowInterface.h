#pragma once

#include "WindowListener.h"

#include "utils/System.h"
#include "utils/Vec2d.h"

#include <vector>


class Chart;

class WindowInterface
{
public:
    virtual ~WindowInterface();

    Chart *TheChart() { return m_pChart; }

    unsigned char ChartIndex();

    void *m_pHandler;

protected:
    WindowInterface();

    WindowInterface(const WindowInterface &b) = delete;
    
    // One Chart per Window !!!
    Chart *m_pChart;

public:
    static std::vector<WindowInterface *> s_windowList;
};