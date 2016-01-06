#pragma once

#include <vector>
#include "entry/SystemTrayInterface.h"
#include "utils/SubServiceInterface.h"
#include "rendering/GraphicContextInterface.h"
#include "utils/System.h"

class WindowInterface;
class Chart;

class WindowService : public SubServiceInterface
{
private:
    WindowService();
    ~WindowService();

public:
    static WindowService &Instance() {
        static WindowService g_WindowService;
        return g_WindowService;
    }

    virtual bool Initialize() override;

    virtual bool DeInitialize() override;
    
    WindowInterface *CreateOneWindow();



    GraphicContextInterface *GraphicContext();

    Chart * FindByIndex(const unsigned char chartIndex);

    bool CloseWindow(const unsigned char chartIndex);

    void CloseAllWindow();

    SystemTrayInterface *GetSystemTray() { return m_pTray; }

    Vec2dFloat m_dpi;


private:
    SystemTrayInterface *m_pTray;

    GraphicContextInterface *m_pGraphicContext;

};