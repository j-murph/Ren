#include "stdafx.h"
#include "Win32Helpers.h"

POINT GetMouseCoordinates(HWND hwnd)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	p.y = clientRect.bottom - p.y;

	return p;
}

void CenterCursorPosition(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	POINT point = { rect.right / 2, rect.bottom / 2 };

	ClientToScreen(hwnd, &point);
	SetCursorPos(point.x, point.y);
}

bool WindowsClassExists(LPCTSTR className)
{
	WNDCLASSEX unused{};
	return GetClassInfoEx(instanceHandle, className, &unused) == TRUE;
}
