#include "WindowInterface.h"
#include "model/Chart.h"
#include "utils/Log.h"

std::vector<WindowInterface *> WindowInterface::s_windowList(0);

WindowInterface::WindowInterface() :
    m_pHandler(nullptr)
{
    WindowInterface::s_windowList.push_back(this);

    m_pChart = new Chart(this);
}

WindowInterface::~WindowInterface()
{
    if (m_pHandler == nullptr) {
        Log::e() << "WindowInterface::~WindowInterface() : m_pHandler == nullptr";
    }
    else {
        delete m_pHandler;
    }

    delete m_pChart;
}

unsigned char WindowInterface::ChartIndex()
{
    return m_pChart->GetIndex();
}
