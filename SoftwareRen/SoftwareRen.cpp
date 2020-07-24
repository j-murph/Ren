// SoftwareRen.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SoftwareRen.h"
#include "Rasterizer.h"
#include "SceneRenderer.h"

#define MAX_LOADSTRING 100
#define MAX_FPS 60

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

Camera mainCamera;

std::unique_ptr<SRGraphicsContext> pSrgc;
std::unique_ptr<Rasterizer> pRasterizer(new Rasterizer);
std::unique_ptr<SceneRenderer> pSceneRenderer(new SceneRenderer(pRasterizer.get()));

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOFTWAREREN, szWindowClass, MAX_LOADSTRING);
	
	bool registeredClass = MyRegisterClass(hInstance) != 0;
	_ASSERT_EXPR(registeredClass, TEXT("Failed to register class."));

	HWND hwnd = InitWindow(hInstance, nCmdShow);
	_ASSERT_EXPR(hwnd != nullptr, TEXT("Failed to initialise window."));

	pSrgc.reset(InitGraphics(hwnd));
	_ASSERT_EXPR(pSrgc.get() != nullptr, TEXT("Failed to create graphics context."));

    int returnCode = MessageLoop(hwnd, hInstance);
	return returnCode;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize			= sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTWAREREN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOFTWAREREN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
   HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hwnd)
   {
      return nullptr;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   return hwnd;
}

SRGraphicsContext* InitGraphics(HWND hwnd)
{
	SRGraphicsContext* srgc = new SRGraphicsContext(hwnd);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (!srgc->frameBuffer->Init(GetDC(hwnd), clientRect.right, clientRect.bottom))
	{
		SAFE_FREE(srgc);
		return nullptr;
	}

	srgc->zBuffer->Init(clientRect.right, clientRect.bottom);

	return srgc;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				switch (wmId)
				{
				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
				}
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				if (pSrgc) pSrgc->frameBuffer->Draw();
				EndPaint(hwnd, &ps);
			}
			break;
		case WM_SIZE:
			{
				RECT clientRect;
				GetClientRect(hwnd, &clientRect);

				if (pSrgc)
				{
					bool resizeSuccess = pSrgc->frameBuffer->SetSize(clientRect.right, clientRect.bottom);
					_ASSERT(resizeSuccess);
					pSrgc->zBuffer->SetSize(clientRect.right, clientRect.bottom);
				}

				mainCamera.SetScreenWidth(clientRect.right);
				mainCamera.SetScreenHeight(clientRect.bottom);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int MessageLoop(HWND hwnd, HINSTANCE hInstance)
{
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTWAREREN));

	Mesh testMesh;
	testMesh.CreateCube(1.2f);
	bool meshLoaded = testMesh.LoadFromFile("..\\Assets\\Models\\monkey2.obj");
	_ASSERT_EXPR(meshLoaded, TEXT("Failed to load mesh"));

	testMesh.SetPosition({ 0, 0, 3.0f });
	pSceneRenderer->AddObjectToScene(&testMesh);

	mainCamera.SetPosition({ 0, 0, 5 });
	mainCamera.LookAt({ 0, 100, 0 });

	pSceneRenderer->SetCamera(&mainCamera);

	g_Timer.Reset();

	Timer fpsLimiter;
	Timer fpsCounter;

	while (true)
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

		const float frameTickRate = 1.0f / MAX_FPS;
		const bool needsRender = fpsLimiter.Elapsed() >= frameTickRate;
		if (needsRender)
		{
			// Lock in the time since the last render
			g_Timer.Lock();

			testMesh.SetPosition({ 0, std::sin(g_Timer.GetLockedTime()) * 2, 0 });
			mainCamera.SetPosition({ 0, 0, 5 });
			mainCamera.LookAt({0, 0, 0});

			Render(hwnd);
			InvalidateRect(hwnd, nullptr, false);

			int currentFps = (int)(1000.0f / (fpsCounter.Elapsed() * 1000) + .5f);
			fpsCounter.Reset();
			UpdateTitle(hwnd, currentFps);

			g_Timer.Reset();
			fpsLimiter.Reset();
		}
	}
}

void Render(HWND hwnd)
{
	if (!pSrgc) return;
	pSrgc->frameBuffer->Clear(Color(0, 0, 0));
	pSceneRenderer->RenderScene(*pSrgc);
}

void UpdateTitle(HWND hwnd, int currentFps)
{
	static Timer lastUpdate;

	// Only update at 1hz
	if (lastUpdate.Elapsed() > 1.0f)
	{
		lastUpdate.Reset();

		WCHAR title[256] = { 0 };
		wsprintf(title, TEXT("%s - %i FPS"), szTitle, currentFps);
		SetWindowText(hwnd, title);
	}
}