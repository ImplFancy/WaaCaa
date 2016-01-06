#include "WindowService.h"

#include "model/Chart.h"
#include "utils/System.h"
#include "utils/Log.h"

WindowService::WindowService()
    :m_pGraphicContext(nullptr),
    m_pTray(nullptr)
{

}

WindowService::~WindowService()
{
    if (m_pGraphicContext != nullptr) {
        delete m_pGraphicContext;
        m_pGraphicContext = nullptr;
    }

    if (m_pTray != nullptr) {
        delete m_pTray;
        m_pTray = nullptr;
    }

}


WindowInterface *WindowService::CreateOneWindow()
{
    auto pWindow = System::Factory()->CreateOneWindow();
    
    if (pWindow == nullptr) {
        Log::e() << "WindowService::CreateOneWindow : pWindow == nullptr ";
        return nullptr;
    }

    return pWindow;
}

Chart * WindowService::FindByIndex(const unsigned char chartIndex)
{
    for (auto i = 0; i < WindowInterface::s_windowList.size(); i++) {
        if (chartIndex == WindowInterface::s_windowList[i]->ChartIndex()) {
            // Found it
            return WindowInterface::s_windowList[i]->TheChart();
        }
    }
    return nullptr;
}

bool WindowService::CloseWindow(const unsigned char chartIndex)
{
    for (auto i = 0; i < WindowInterface::s_windowList.size(); i++) {
        if (chartIndex == WindowInterface::s_windowList[i]->ChartIndex()) {
            // Found it
            delete WindowInterface::s_windowList[i];
            WindowInterface::s_windowList.erase(WindowInterface::s_windowList.begin() + i);
            return true;
        }
    }

    // Not Founded
    return false;
}

void WindowService::CloseAllWindow()
{
    for (auto pWindow : WindowInterface::s_windowList) {
        delete pWindow;
    }
    WindowInterface::s_windowList.clear();
}

bool WindowService::Initialize()
{
    m_pTray = System::Factory()->CreateSystemTray();

    // build GraphicContext
    m_pGraphicContext = System::Factory()->CreateGraphicContext();

    return true;
}

bool WindowService::DeInitialize()
{
    if (m_pGraphicContext != nullptr) {
        delete m_pGraphicContext;
        m_pGraphicContext = nullptr;
    }

    if (m_pTray != nullptr) {
        delete m_pTray;
        m_pTray = nullptr;
    }

    return true;
}

GraphicContextInterface * WindowService::GraphicContext()
{
    return m_pGraphicContext;
}
