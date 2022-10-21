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

	RenWindow* window = CreateWin<RenWindow>(1024, 768, TEXT("Ren"));
	_ASSERT_EXPR(window != nullptr, TEXT("Failed to initialise window."));

	return MessageLoop(window);
}

int MessageLoop(RenWindow* window)
{
	AutoReleaser winReleaser{ window };

	HACCEL hAccelTable = LoadAccelerators(GetAppInstance(), MAKEINTRESOURCE(IDC_SOFTWAREREN));

	while (!window->IsDestroyed())
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

		window->Tick();
	}

	return 0;
}
