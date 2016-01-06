#include "SystemTrayWin32.h"
#include "resources/Resource.h"
#include "window/WindowService.h"
#include "window/WindowWin32.h"
#include "utils/SystemEvents.h"
#include "model/Protocol.h"


wchar_t *SystemTrayWin32::sAppName = L"WaaCaa";


SystemTrayWin32::~SystemTrayWin32()
{
    m_hWnd = 0;
}


SystemTrayWin32::SystemTrayWin32()
    :m_hWnd(0)
{
    {
        WNDCLASS wc = { 0 };
        wc.style = NULL;
        wc.hIcon = NULL;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = HINST_THISCOMPONENT;
        wc.lpfnWndProc = WindowWin32::MainProc;
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = sAppName;
        wc.hCursor = NULL;

        if (!RegisterClass(&wc)) return;
    }

    m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, sAppName, sAppName, WS_POPUP, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, HINST_THISCOMPONENT, NULL);


    {
        m_nid.cbSize = sizeof(NOTIFYICONDATA);
        m_nid.hWnd = m_hWnd;
        m_nid.uID = IDI_TRAY;
        m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
        m_nid.uCallbackMessage = SystemEvents::ToSystemEvent(SystemEvents::MESSAGE_TRAY);
        m_nid.hIcon = LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_TRAY));
        lstrcpy(m_nid.szTip, sAppName);

        m_hMainMenu = CreatePopupMenu();
        AppendMenu(m_hMainMenu, MF_STRING, ID_CLOSE_ALL, TEXT("Close All Charts"));
        AppendMenu(m_hMainMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(m_hMainMenu, MF_STRING, ID_SETTING, TEXT("Setting"));
        AppendMenu(m_hMainMenu, MF_STRING, ID_EXIT, TEXT("Exit WaaCaa Service"));

        Shell_NotifyIcon(NIM_ADD, &m_nid);
    }
}


void SystemTrayWin32::ShowMessage()
{
    lstrcpy(m_nid.szInfoTitle, sAppName);
    lstrcpy(m_nid.szInfo, TEXT("Get One Message£¡"));
    m_nid.uTimeout = 1000;
    Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

void SystemTrayWin32::ShowAndUpdate()
{
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);
}
