#include "stdafx.h"
#include "Window.h"
#include "resource.h"

bool Window::Create(int width, int height, LPCTSTR title)
{
	auto windowClassAtom = GetWindowClassAtom();
	if (!windowClassAtom)
		return false;

	hwnd = CreateWindow(MAKEINTATOM(windowClassAtom), title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instanceHandle, this);

	if (!hwnd)
	{
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return true;
}

ATOM Window::GetWindowClassAtom()
{
	return defaultRegistrar.GetClassAtom(getInstanceHandle());
}

bool Window::ClassExists(LPCTSTR className)
{
	WNDCLASSEX unused{};
	return GetClassInfoEx(instanceHandle, className, &unused) == TRUE;
}

LRESULT CALLBACK Window::TopLevelWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
	}
	else
	{
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}