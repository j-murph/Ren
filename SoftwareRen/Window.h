#pragma once
#include "WindowsClassRegistrar.h"


class Window
{
private:
	HWND hwnd = nullptr;

	static LRESULT CALLBACK TopLevelWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static inline DefaultWindowsClassRegistrar defaultRegistrar{ Window::TopLevelWndProc };

	static inline ATOM defaultClassAtom = 0;

	static inline HINSTANCE instanceHandle = 0;

protected:
	virtual ATOM GetWindowClassAtom();

	static bool ClassExists(LPCTSTR className);

	static WndProcType GetTopLevelWndProc() { return Window::TopLevelWndProc; }

public:
	Window() = default;

	static void SetInstanceHandle(HINSTANCE hInstance) { instanceHandle = hInstance; }
	static HINSTANCE getInstanceHandle() { return instanceHandle; }

	virtual bool Create(int width, int height, LPCTSTR title);

	HWND GetWindowHandle() const { return hwnd; };

	virtual ~Window() {}
};

class RenWindow : public Window
{

};
