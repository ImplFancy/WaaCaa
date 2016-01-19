#include "SystemEvents.h"
#include "utils/Common.h"
#include "window/WindowService.h"
#include "entry/SystemTrayWin32.h"

#ifdef TARGET_WINDOWS
#include <Windows.h>
#endif


unsigned int SystemEvents::ToSystemEvent(unsigned short userEvent)
{
    return WM_APP + userEvent;
}

unsigned short SystemEvents::ToUserEvent(unsigned int sysEvent)
{
    return sysEvent - WM_APP;
}

SystemEvents::RetType SystemEvents::SendOneMessage(unsigned int message, const char *pParam, const char *lParam)
{
    RetType ret = RetType::OK;
#ifdef TARGET_WINDOWS
    auto pTray = (SystemTrayWin32 *)(WindowService::Instance().GetSystemTray());
    ret = (RetType) SendMessage(pTray->m_hWnd, ToSystemEvent(message), (WPARAM)pParam, (LPARAM)lParam);
#endif // TARGET_WINDOWS
    return ret;
}


