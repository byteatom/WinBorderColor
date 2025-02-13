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

    auto cwpRet = (PCWPRETSTRUCT)lParam;
    auto msg = cwpRet->message;
    auto hwnd = cwpRet->hwnd;

    if (nCode >= 0
        && (msg == WM_ACTIVATE || msg == WM_ACTIVATEAPP || msg == WM_NCACTIVATE || msg == WM_SETFOCUS || msg == WM_KILLFOCUS)
        && !(GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD)
        && !GetParent(hwnd)
        )
    {
        const COLORREF borderColor = 0x00008800;
        auto result = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));
        if (result != S_OK)
            winLog("DwmSetWindowAttribute:{:x}", result);
    }

    return CallNextHookEx(0, nCode, wParam, lParam);
}

