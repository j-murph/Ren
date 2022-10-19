#pragma once

using WndProcType = LRESULT (CALLBACK*)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class WindowsClassRegistrar
{
	ATOM classAtom = 0;

	LPCTSTR className = nullptr;

	WndProcType wndProc = nullptr;

protected:
	WndProcType GetWndProc() { return wndProc; }

	virtual ATOM RegisterWindowsClass(const LPCTSTR className, HINSTANCE appInstance) = 0;

public:
	WindowsClassRegistrar(const LPCTSTR className, WndProcType wndProc);
	virtual ~WindowsClassRegistrar();

	LPCTSTR GetClassName() const { return className; }

	ATOM GetClassAtom();
};

class DefaultWindowsClassRegistrar : public WindowsClassRegistrar
{
protected:
	ATOM RegisterWindowsClass(const LPCTSTR className, HINSTANCE appInstance) override;

public:
	DefaultWindowsClassRegistrar(WndProcType wndProc)
		: WindowsClassRegistrar(TEXT("RenDefaultWin_0195302"), wndProc)
	{}
};