#include <windows.h>
#include <winuser.h>
#include "conf.h"
#include <nlohmann/json.hpp>


typedef int (*GetCurrentDesktopNumberFn)();
typedef void (*GoToDesktopNumberFn)(int);
typedef void (*MoveWindowToDesktopNumberFn)(HWND, int);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	wchar_t path[32767];
	HMODULE lib = LoadLibraryEx(L"virtual-desktop-accessor.dll", NULL, NULL);

	if (!lib) // Уведомление если нет библиотеки в PATH или папке программы
	{
		DWORD err = GetLastError();
		GetEnvironmentVariableW(L"PATH", path, _countof(path));
		MessageBoxW(NULL, path, L"virtual-desktop-accessor.dll not in PATH", MB_OK);
		return 100;
	}

	auto GetCurrentDesktopNumber =
		(GetCurrentDesktopNumberFn)GetProcAddress(lib, "GetCurrentDesktopNumber");

	auto GoToDesktopNumber =
		(GoToDesktopNumberFn)GetProcAddress(lib, "GoToDesktopNumber");

	auto MoveWindowToDesktopNumber =
		(MoveWindowToDesktopNumberFn)GetProcAddress(lib, "MoveWindowToDesktopNumber");

	if (!GetCurrentDesktopNumber || !GoToDesktopNumber || !MoveWindowToDesktopNumber) {
		return 101;
	}

	int DesktopNum = GetCurrentDesktopNumber();  // Запоминаем текущий стол

	using json = nlohmann::json;
	json config_parsed = config("config.json"); // Читаем конфиг биндов

	// Alt + 1..9 (id 1->9)
	for (int i = 1; i <= 9; i++) {
		RegisterHotKey(NULL, i, config_parsed["swap desktops"]["modifiers"], '0' + i);
	}

	// Alt + Shift + 1..9 (id 101->109)
	for (int i = 1; i <= 9; i++) {
		RegisterHotKey(NULL, 100 + i, config_parsed["swap window to desktop"]["modifiers"], '0' + i);
	}

	// ALT+CTRL+Q hotkey (id 11) Close Window
	RegisterHotKey(NULL, 11, config_parsed["close window"]["modifiers"], config_parsed["close window"]["key"]);


	// ALT+CTRL+Shift+Q hotkey (id 12) Terminate Window
	RegisterHotKey(NULL, 12, config_parsed["terminate window"]["modifiers"], config_parsed["terminate window"]["key"]);

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

			else if (id == 11 || id == 12) {
				HWND hActiveWindow = GetForegroundWindow();
				if (hActiveWindow) {
					wchar_t title[256];
					GetWindowTextW(hActiveWindow, title, 256);
					//MessageBoxW(NULL, title, L"close window", MB_OK);
					if (id == 11) {
						PostMessageW(hActiveWindow, WM_CLOSE, 0, 0);
					}
					else if (id == 12) {
						DWORD pid = 0;
						GetWindowThreadProcessId(hActiveWindow, &pid);
						if (pid || pid != 0) {
							HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
							if (hProcess) {
								TerminateProcess(hProcess, 0);
								CloseHandle(hProcess);
							}
						}
					}
				}
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
		UnregisterHotKey(NULL, 11);
		UnregisterHotKey(NULL, 12);
	}
	FreeLibrary(lib);
	return 0;
}