#include <windows.h>


typedef int (*GetCurrentDesktopNumberFn)();
typedef void (*GoToDesktopNumberFn)(int);
typedef void (*MoveWindowToDesktopNumberFn)(HWND, int);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	HMODULE lib = LoadLibraryEx(L"virtual-desktop-accessor.dll", NULL, NULL);

	if (!lib) {
		return 10;
	}

	auto GetCurrentDesktopNumber =
		(GetCurrentDesktopNumberFn)GetProcAddress(lib, "GetCurrentDesktopNumber");

	auto GoToDesktopNumber =
		(GoToDesktopNumberFn)GetProcAddress(lib, "GoToDesktopNumber");

	auto MoveWindowToDesktopNumber =
		(MoveWindowToDesktopNumberFn)GetProcAddress(lib, "MoveWindowToDesktopNumber");

	if (!GetCurrentDesktopNumber || !GoToDesktopNumber || !MoveWindowToDesktopNumber) {
		return 100;
	}

	int DesktopNum = GetCurrentDesktopNumber();

	// Alt + 1..9 (id 1->9)
	for (int i = 1; i <= 9; i++) {
		RegisterHotKey(NULL, i, MOD_ALT, '0' + i);
	}

	// Alt + Shift + 1..9 (id 101->109)
	for (int i = 1; i <= 9; i++) {
		RegisterHotKey(NULL, 100 + i, MOD_ALT | MOD_SHIFT, '0' + i);
	}

	// Считывание клавиш
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_HOTKEY) {
			int id = msg.wParam; // ID = номер (1..9)

			if (id >= 1 && id <= 9) {

				HWND oldWindow = GetForegroundWindow();

				if (id - 1 != GetCurrentDesktopNumber()) { // проверка, что переключаешься на другой стол

					GoToDesktopNumber(id - 1); // обычно десктопы с 0

					Sleep(50);
					HWND shell = GetShellWindow();
					SetForegroundWindow(shell);
				};
			}

			else if (id >= 101 && id <= 109) {
				HWND window = GetForegroundWindow();
				if (window) {
					MoveWindowToDesktopNumber(window, id - 101);
					GoToDesktopNumber(id - 101);
				}
			}
		}
	}

	// cleanup
	for (int i = 1; i <= 9; i++) {
		UnregisterHotKey(NULL, i);
		UnregisterHotKey(NULL, i + 100);
	}
	FreeLibrary(lib);
	return 0;
}