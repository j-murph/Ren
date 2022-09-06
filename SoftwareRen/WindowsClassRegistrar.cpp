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
	if (--referenceCount == 0)
	{
		UnregisterClass(className, appInstance);
		classAtom = 0;
		appInstance = 0;
	}
}

ATOM WindowsClassRegistrar::GetClassAtom(HINSTANCE appInstance, bool addReference /* = true */)
{
	if (classAtom == 0)
	{
		this->appInstance = appInstance;
		classAtom = RegisterWindowsClass(className, appInstance);
		referenceCount++;
	}
	else if (addReference)
	{
		referenceCount++;
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
