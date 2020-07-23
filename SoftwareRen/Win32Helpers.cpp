#include "stdafx.h"
#include "Win32Helpers.h"

POINT GetMouseCoordinates(const HWND& hwnd)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	p.y = clientRect.bottom - p.y;

	return p;
}
