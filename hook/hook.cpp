#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

#include "winutil.h"

extern "C" LRESULT CALLBACK callWndProcRet(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
) {
    
    #pragma EXPORT_FUNC

    auto cwpRets = (PCWPRETSTRUCT)lParam;
    auto msg = cwpRets->message;
    auto hwnd = cwpRets->hwnd;
    auto style = GetWindowLong(hwnd, GWL_STYLE);

    if (nCode < 0
        || !((style & WS_OVERLAPPED) || (style & WS_OVERLAPPEDWINDOW))
        || !(msg == WM_ACTIVATE || msg == WM_ACTIVATEAPP || msg == WM_SHOWWINDOW))
        return CallNextHookEx(0, nCode, wParam, lParam);

    const COLORREF borderColor = 0x00008800;
    auto result = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
    if (result != S_OK)
        winLog("DwmSetWindowAttribute:{:x}", result);

    return CallNextHookEx(0, nCode, wParam, lParam);
}

