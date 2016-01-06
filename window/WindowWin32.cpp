#include "WindowWin32.h"
#include "WindowService.h"

#include "entry/SystemTrayWin32.h"
#include "model/Protocol.h"
#include "model/Chart.h"
#include "resources/Resource.h"
#include "settings/RenderringSettings.h"
#include "utils/Log.h"
#include "utils/SystemEvents.h"

#include <Windows.h>


WindowWin32::WindowWin32(unsigned int w, unsigned int h)
{
    HRESULT hr;

    // Register the window class.
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowWin32::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"DataVisualization";

    auto ret = RegisterClassEx(&wcex);

    // Create the application window.
    //
    // Because the CreateWindow function takes its size in pixels, we
    // obtain the system DPI and use it to scale the window size.
    // TODO: beautify the code
    WindowService::Instance().GraphicContext()->GetDesktopDpi(WindowService::Instance().m_dpi);

    auto hWnd = CreateWindow(
        L"DataVisualization",
        L"Data Visualization",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<UINT>(ceil(w * WindowService::Instance().m_dpi.x / 96.f)),
        static_cast<UINT>(ceil(h * WindowService::Instance().m_dpi.y / 96.f)),
        NULL,
        NULL,
        HINST_THISCOMPONENT,
        this
        );

    hr = hWnd ? S_OK : E_FAIL;
    
    if (SUCCEEDED(hr))
    {
        // must before Show and Update
        m_pHandler = new HWND(hWnd);

        ShowWindow(hWnd, SW_SHOWNORMAL);
        UpdateWindow(hWnd);
        SetForegroundWindow(hWnd);
    }

}

WindowWin32::WindowWin32() :
    WindowWin32(640, 480)
{
}

WindowWin32::~WindowWin32()
{
    HWND hWnd = *((HWND *)(m_pHandler));
    PostMessage(hWnd, WM_CLOSE, 0, 0);
}

WindowWin32 * WindowWin32::FindWindowByHandler(HWND hWnd)
{
    WindowWin32 *theWindow = nullptr;
    for (auto pWindow : WindowInterface::s_windowList) {
        auto p_HWND = (HWND *)pWindow->m_pHandler;
        if (p_HWND != nullptr) {
            if (hWnd == *p_HWND) {
                theWindow = (WindowWin32 *)pWindow;
                break;
            }
        }
    }
    return theWindow;
}

Chart * WindowWin32::FindChartByHandler(HWND hWnd)
{
    WindowWin32 *theWindow = FindWindowByHandler(hWnd);

    if (theWindow == nullptr) return nullptr;

    return theWindow->TheChart();;
}

LRESULT CALLBACK WindowWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Chart *pChart = FindChartByHandler(hWnd);
    if (pChart != nullptr) {
        unsigned char index = pChart->GetIndex();
    }

    // To be settable
    static bool isImmediateMode = RenderringSettings::Get().IsImmediateMode();

    // FIXME: to force render multiple windows
    auto forceRender = [](HWND hWnd) {
        InvalidateRect(hWnd, NULL, FALSE);
    };

    switch (message) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        if (pChart != nullptr) pChart->OnRender();
        EndPaint(hWnd, &ps);
        if (isImmediateMode) forceRender(hWnd);
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (pChart != nullptr) pChart->OnMouseMove(
            Vec2dFloat(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
            wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON
            );
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if (pChart != nullptr) {
            POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hWnd, &point);
            pChart->OnWheel(Vec2dFloat(point.x, point.y), GET_WHEEL_DELTA_WPARAM(wParam));
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (pChart != nullptr) {
            POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hWnd, &point);
            pChart->OnMouseLButtonDown(point.x, point.y);
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (pChart != nullptr) {
            POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hWnd, &point);
            pChart->OnMouseLButtonUp(point.x, point.y);
        }
        break;
    }
    case WM_KEYDOWN:
    {
        if (pChart != nullptr) pChart->OnKeyDown(static_cast<SHORT>(wParam));
        break;
    }
    case WM_SIZE:
    {
        if (pChart != nullptr) pChart->OnResize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_DESTROY:
    {
        if (pChart != nullptr) pChart->OnDestroy();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    if (!isImmediateMode) {
        if (message != WM_DESTROY&&message != WM_PAINT) {
            forceRender(hWnd);
        }
    }

    return 0;
}


LRESULT CALLBACK WindowWin32::MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg >= WM_USER && uMsg <= WM_USER + 0xff) {
        SetForegroundWindow(hWnd);

        auto userEvent = SystemEvents::ToUserEvent(uMsg);
        Protocol::ProcessRequestBody(
            (LinearBuffer::Static *)wParam,
            userEvent >> 5, userEvent & 0x1F,
            (LinearBuffer *)lParam);
        return SystemEvents::OK;
    }
    else {
        // System Tray About
        auto pTray = (SystemTrayWin32 *)(WindowService::Instance().GetSystemTray());

        if (uMsg == SystemEvents::ToSystemEvent(SystemEvents::MESSAGE_TRAY)) {
            if (lParam == WM_RBUTTONDOWN) {
                POINT pt;
                GetCursorPos(&pt);			

                int cmd = TrackPopupMenu(
                    pTray->m_hMainMenu,
                    TPM_RETURNCMD,
                    pt.x, pt.y,
                    NULL, hWnd, NULL);

                if (cmd == ID_CLOSE_ALL) {
                    WindowService::Instance().CloseAllWindow();
                }
                else if (cmd == ID_SETTING) {
                    int i(3);
                    // PostMessage(hWnd, WM_SHOWTRAYMSG, NULL, NULL);
                }
                else if (cmd == ID_EXIT) {
                    PostMessage(hWnd, WM_DESTROY, NULL, NULL);
                }
            }
        }
        else if (uMsg == WM_DESTROY) {
            Shell_NotifyIcon(NIM_DELETE, &(pTray->m_nid));
            PostQuitMessage(0);
        }
        else if (uMsg == RegisterWindowMessage(TEXT("TaskbarCreated"))) {
            // Reload System Tray when Explorer Crashed
            Shell_NotifyIcon(NIM_ADD, &(pTray->m_nid));
        }
        else {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
}
