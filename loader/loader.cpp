#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "winutil.h"

//The window and message loop are designed to wait for taskkill's WM_CLOSE and exit gracefully by calling UnhookWindowsHookEx

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    const wchar_t className[] = L"MainWnd";    

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = MainWndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = className;
    RegisterClassExW(&wcex);

    auto wnd = CreateWindowW(className, L"", 0,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_MESSAGE, nullptr, hInstance, nullptr);
    if (!wnd) {
        winLogLastError("CreateWindowW");
        return FALSE;
    }

    USHORT processMachine;
    USHORT nativeMachine;
    if (!IsWow64Process2(GetCurrentProcess(), &processMachine, &nativeMachine)) {
        winLogLastError("IsWow64Process2");
        return FALSE;
    }
    auto dll = LoadLibraryW(processMachine == IMAGE_FILE_MACHINE_I386 ? L"hook32.dll" : L"hook64.dll");
    if (!dll) {
        winLogLastError("LoadLibraryW");
        return FALSE;
    }

    auto callWndProcRet = (HOOKPROC)GetProcAddress(dll, "callWndProcRet");
    if (!callWndProcRet) {
        winLogLastError("GetProcAddress");
        return FALSE;
    }

    auto callWndProcRetHook = SetWindowsHookExW(WH_CALLWNDPROCRET, callWndProcRet, dll, 0);
    if (!callWndProcRetHook) {
        winLogLastError("SetWindowsHookExW");
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);        
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(callWndProcRetHook);

    return (int)msg.wParam;
}
