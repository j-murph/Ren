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

	AllocDebugConsole();
	SetAppInstance(hInstance);

	RenWindow* window = CreateWin<RenWindow>(1024, 768, TEXT("Ren"));
	_ASSERT_EXPR(window != nullptr, TEXT("Failed to initialise window."));

	const int returnCode = MessageLoop(window);

	return returnCode;
}

int MessageLoop(RenWindow* window)
{
	AutoReleaser winReleaser{window};

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
