#include "stdafx.h"
#include "Window.h"
#include "resource.h"
#include "Win32Helpers.h"

Window::~Window()
{
}

bool Window::Create(int width, int height, LPCTSTR title)
{
	ATOM windowClassAtom = GetWindowClassAtom();
	if (!windowClassAtom)
		return false;

	HWND result = CreateWindow(MAKEINTATOM(windowClassAtom), title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, GetAppInstance(), this);

	if (!result)
	{
		return false;
	}

	return true;
}

void Window::Destroy()
{
	DestroyWindow(GetWindowHandle());
}

ATOM Window::GetWindowClassAtom()
{
	return defaultRegistrar.GetClassAtom();
}

LRESULT CALLBACK Window::DefaultWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* targetWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (targetWindow)
	{
		if (message == WM_NCDESTROY)
		{
			LRESULT retValue = targetWindow->HandleMessage(message, wParam, lParam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

			targetWindow->destroyed = true;
			targetWindow->Release();

			return retValue;
		}

		return targetWindow->HandleMessage(message, wParam, lParam);
	}
	
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		targetWindow = static_cast<Window*>(cs->lpCreateParams);
		targetWindow->hwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int Window::AddRef()
{
	return ++refCount;
}

void Window::Release()
{
	if (--refCount == 0)
	{
		delete this;
	}
}

HWND Window::GetWindowHandle() const
{
	return hwnd;
}

bool Window::IsDestroyed() const
{
	return destroyed;
}
