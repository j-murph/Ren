#pragma once

POINT GetMouseCoordinates(HWND hwnd);

void CenterCursorPosition(HWND hwnd);

bool WinClassExists(HINSTANCE instanceHandle, LPCTSTR className);

void SetAppInstance(HINSTANCE handle);

HINSTANCE GetAppInstance();