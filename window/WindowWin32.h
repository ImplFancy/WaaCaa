#pragma once
#include "WindowInterface.h"
#include "utils/Vec2d.h"

#include <Windows.h>
#include <windowsx.h>


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


class WindowWin32 : public WindowInterface
{
public:

    WindowWin32();

    WindowWin32(unsigned int w, unsigned int h);

    ~WindowWin32();

    static inline WindowWin32 *FindWindowByHandler(HWND hWnd);

    static inline Chart *FindChartByHandler(HWND hWnd);

    static LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};