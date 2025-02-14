#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>

#include "winutil.h"

const HMODULE getCurrentModule()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
            | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)getCurrentModule, &hModule);
    return hModule;
}

inline fs::path getCfgPath() {
    return getModuleDir(getCurrentModule()) / _T("WinBorderColor.ini");
}

UINT getIntCfg(LPCWSTR lpKeyName, INT nDefault) {
    static const fs::path cfgPath = getCfgPath();
    return GetPrivateProfileInt(_T("app"), lpKeyName, nDefault, cfgPath.c_str());
}

extern "C" LRESULT CALLBACK callWndProcRet(
    int    nCode,
    WPARAM wParam,
    LPARAM lParam
) {

    #pragma EXPORT_FUNC

    static auto foregroundColor = getIntCfg(_T("foregroundColor"), 0x00000088);
    static auto backgroundColor = getIntCfg(_T("backgroundColor"), 0x00009900);

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
            ? foregroundColor
            : backgroundColor;
        auto result = DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &color, sizeof(color));
        if (result != S_OK)
            winLog("DwmSetWindowAttribute:{:x}", result);
    }

    return CallNextHookEx(0, nCode, wParam, lParam);
}

