#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

#include "winutil.h"

HMODULE getCurrentModule()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
            | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)getCurrentModule, &hModule);
    return hModule;
}

inline auto getCfgPath() {
    return getModuleDir(getCurrentModule()) / _T("WinBorderColor.ini");
}

auto getIntCfg(LPCWSTR lpKeyName, INT nDefault) {
    static const auto cfgPath = getCfgPath();
    return GetPrivateProfileInt(_T("app"), lpKeyName, nDefault, cfgPath.c_str());
}

VOID CALLBACK timerProc(
    HWND hwnd, 
    UINT msg,
    UINT_PTR id,
    DWORD time)
{
    HRESULT result;
    auto color = (LPCOLORREF)id;
    result = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, color, sizeof(*color));
    if (result != S_OK)
        winLog("DwmSetWindowAttribute:{:x}", result);
    KillTimer(hwnd, id);
}

extern "C" LRESULT CALLBACK callWndProcRet(
    int    nCode,
    WPARAM wParam,
    LPARAM lParam
) {
    #pragma EXPORT_FUNC

    static const COLORREF fgColor = getIntCfg(_T("foregroundColor"), 0x00000088);
    static const COLORREF bgColor = getIntCfg(_T("backgroundColor"), 0x00009900);

    auto cwpRet = (PCWPRETSTRUCT)lParam;
    auto msg = cwpRet->message;
    auto hwnd = cwpRet->hwnd;

    if (nCode >= 0
        && (msg == WM_ACTIVATE || msg == WM_ACTIVATEAPP || msg == WM_NCACTIVATE || msg == WM_SETFOCUS || msg == WM_KILLFOCUS)
        && !(GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD)
        && !GetParent(hwnd)
        )
    {
        auto wParamMsg = cwpRet->wParam;
        auto color = (msg == WM_ACTIVATE && LOWORD(wParamMsg) != WA_INACTIVE)
                || ((msg == WM_ACTIVATEAPP || msg == WM_NCACTIVATE) && wParamMsg)
                || (msg == WM_SETFOCUS)
            ? &fgColor
            : &bgColor;
        HRESULT result;

        result = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, color, sizeof(*color));
        if (result != S_OK)
            winLog("DwmSetWindowAttribute:{:x}", result);

        KillTimer(hwnd, (UINT_PTR)&fgColor);
        KillTimer(hwnd, (UINT_PTR)&bgColor);
        result = SetTimer(hwnd,
            (UINT_PTR)color,
            1000,
            (TIMERPROC)timerProc);
        if (result == 0)
            winLogLastError("SetTimer");
    }

    return CallNextHookEx(0, nCode, wParam, lParam);
}

