#include "stdafx.h"
#include "Win32Helpers.h"
#include "SoftwareRen.h"

int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SetAppInstance(hInstance);
	AllocDebugConsole();

	std::shared_ptr<int> p = std::make_shared<int>(5);
	auto gg = p;

	RenWindow* mainWindow = RenWindow::CreateInstance();
	bool windowCreated = mainWindow->Create(1024, 768, TEXT("Ren"));
	_ASSERT_EXPR(windowCreated, TEXT("Failed to initialise window."));

	return MessageLoop(mainWindow);
}

int MessageLoop(RenWindow* mainWindow)
{
	AutoReleaser winReleaser{ mainWindow };

	HACCEL hAccelTable = LoadAccelerators(GetAppInstance(), MAKEINTRESOURCE(IDC_SOFTWAREREN));

	while (!mainWindow->IsDestroyed())
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
				return (int)msg.wParam;
		}

		mainWindow->Tick();
	}

	return 0;
}
