
#include "utils/System.h"

#ifdef TARGET_WINDOWS

#include <Windows.h>
#include "network/NetworkService.h"
#include "resources/resource.h"
#include "window/WindowService.h"


void MainLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR, int )
{
    if (FAILED(CoInitialize(NULL))) {
        Log::e() << "CoInitialize failed!";
        return -1;
    }

    if (WindowService::Instance().Initialize()) {
        if (NetworkService::Instance().Initialize()) {
            MainLoop();
            NetworkService::Instance().DeInitialize();
        }
        WindowService::Instance().DeInitialize();
    }

    CoUninitialize();

    return 0;
}


#endif // TARGET_WINDOWS
