#pragma once

#include <Windows.h>

#include "SystemTrayInterface.h"

class SystemTrayWin32 : public SystemTrayInterface
{
private:
    friend class WindowService;
    friend class FactoryWin32;

    SystemTrayWin32();
    ~SystemTrayWin32();

public:
    void ShowAndUpdate();

    void ShowMessage();

    HWND m_hWnd;
    NOTIFYICONDATA m_nid;		// Property of Tray
    HMENU m_hMainMenu;
};

