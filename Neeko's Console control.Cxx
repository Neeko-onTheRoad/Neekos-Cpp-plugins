#define _WIN32_WINNT 0x0500
#include <Windows.h>

void disableConsoleResize() {
	HWND console = GetConsoleWindow();
	SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	return;
}

void setConsoleSize(int h, int w) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, w, h, TRUE);
	return;
}

void setConsoleLocation(int x, int y) {
	HWND console = GetConsoleWindow();
	SetWindowPos(console, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return;
}

void disableUserSelection() {
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prevMode;

	GetConsoleMode(hInput, &prevMode);
	SetConsoleMode(hInput,
		ENABLE_EXTENDED_FLAGS |
		(prevMode & ~ENABLE_QUICK_EDIT_MODE)
	);
	return;
}

void setCursorBlinking(bool status) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = status;
	SetConsoleCursorInfo(hOut, &cursorInfo);
	return;
}

void disableScrollBar() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOut, &info);
	COORD new_size = {
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(hOut, new_size);
}

void setConsoleCloseButton(bool status) {
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, status ? MF_ENABLED : MF_GRAYED);
	return;
}

void setConsoleMinimizeButton(bool status) {
	HWND hwnd = GetConsoleWindow();
	SetWindowLong(hwnd, GWL_STYLE,
		status ? (GetWindowLong(hwnd, GWL_STYLE) | WS_MINIMIZEBOX) :
		(GetWindowLong(hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX)
	);
	return;
}

void setConsoleVisible(bool status) {
	HWND hwnd;
	AllocConsole();
	hwnd = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hwnd, status);
	return;
}