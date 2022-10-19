#pragma once
#include "WindowsClassRegistrar.h"
#include "Rasterizer.h"
#include "SceneRenderer.h"
#include "IRefCounted.h"

class Window : public IRefCounted
{
private:
	bool destroyed = false;

	int refCount = 1;

	HWND hwnd = nullptr;

	static LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static inline DefaultWindowsClassRegistrar defaultRegistrar{ Window::DefaultWndProc };

protected:
	Window() = default;

	virtual ATOM GetWindowClassAtom();

	virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

public:
	virtual ~Window();

	virtual bool Create(int width, int height, LPCTSTR title);

	virtual void Destroy();

	virtual int AddRef() final;
	virtual void Release() final;

	HWND GetWindowHandle() const;

	bool IsDestroyed() const;
};
