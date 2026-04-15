#include <windows.h>
#include <iostream>

typedef int (*GetCurrentDesktopNumberFn)();
typedef void (*GoToDesktopNumberFn)(int);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    HMODULE lib = LoadLibrary(L"virtual-desktop-accessor.dll");

    if (!lib) {
        return 1;
    }

    auto GetCurrentDesktopNumber =
        (GetCurrentDesktopNumberFn)GetProcAddress(lib, "GetCurrentDesktopNumber");

    auto GoToDesktopNumber =
        (GoToDesktopNumberFn)GetProcAddress(lib, "GoToDesktopNumber");

    if (!GetCurrentDesktopNumber || !GoToDesktopNumber) {
        return 1;
    }

    // Alt + 1..9
    for (int i = 1; i <= 9; i++) {
        RegisterHotKey(NULL, i, MOD_ALT, '0' + i);
    }

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            int desktop = msg.wParam; // ID = номер (1..9)
            GoToDesktopNumber(desktop - 1); // обычно десктопы с 0
        }
    }

    // cleanup
    for (int i = 1; i <= 9; i++) {
        UnregisterHotKey(NULL, i);
    }

    CoUninitialize();
    FreeLibrary(lib);

    return 0;
}