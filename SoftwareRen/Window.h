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
	virtual ~Window();

	virtual ATOM GetWindowClassAtom();

	virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

public:
	virtual bool Create(int width, int height, LPCTSTR title);

	virtual void Destroy();

	int AddRef() final;
	void Release() final;

	HWND GetWindowHandle() const;

	bool IsDestroyed() const;
};

template <class T, typename ...A>
T* CreateWin(A&& ...args)
{
	T* window = new T();
	if (!window->Create(std::forward<A>(args)...))
	{
		// No need to Release() window
		return nullptr;
	}

	return window;
}
