#include "stdafx.h"
#include "WindowsClassRegistrar.h"
#include "Win32Helpers.h"
#include "resource.h"

WindowsClassRegistrar::WindowsClassRegistrar(const LPCTSTR className, WndProcType wndProc)
	: className(className), wndProc(wndProc)
{
}

WindowsClassRegistrar::~WindowsClassRegistrar()
{
	if (classAtom != 0)
	{
		classAtom = 0;
		UnregisterClass(className, GetAppInstance());
	}
}

ATOM WindowsClassRegistrar::GetClassAtom()
{
	if (classAtom == 0)
	{
		classAtom = RegisterWindowsClass(className, GetAppInstance());
	}

	_ASSERT(classAtom != 0);

	return classAtom;
}

ATOM DefaultWindowsClassRegistrar::RegisterWindowsClass(const LPCTSTR className, HINSTANCE appInstance)
{
	WNDCLASSEXW wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GetWndProc();
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = appInstance;
	wcex.hIcon = LoadIcon(appInstance, MAKEINTRESOURCE(IDI_SOFTWAREREN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SOFTWAREREN);
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}
