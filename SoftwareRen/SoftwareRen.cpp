// SoftwareRen.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <wchar.h>
#include "SoftwareRen.h"
#include "Rasterizer.h"
#include "SceneRenderer.h"

#define MAX_LOADSTRING 100
#define MAX_FPS 322
#define MOUSE_SENSITIVITY 2.0f

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

   //ShowCursor(FALSE);

   return hwnd;
}

SRGraphicsContext* InitGraphics(HWND hwnd)
{
	SRGraphicsContext* srgc = new SRGraphicsContext(hwnd);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (!srgc->frameBuffer->Init(hwnd, clientRect.right, clientRect.bottom))
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
			{	/*
				int wmId = LOWORD(wParam);
				switch (wmId)
				{
				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
				}
				*/
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				if (pSrgc)
				{
					RECT clientRect;
					GetClientRect(hwnd, &clientRect);

					HDC memDc = CreateCompatibleDC(hdc);
					HBITMAP hBm = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
					SelectObject(memDc, hBm);

					pSrgc->frameBuffer->Draw(memDc);

					const Vert3df& cameraPos = mainCamera.GetPosition();
					const Vert3df& lookAt = mainCamera.GetLookDirection();
					wchar_t szCameraPos[255] = { 0 };
					int characterCount = swprintf_s(szCameraPos, 255, L"Camera Position: %f %f %f\nCamera Look Direction: %f %f %f",
						cameraPos.x, cameraPos.y, cameraPos.z, lookAt.x, lookAt.y, lookAt.z);

					DrawText(memDc, szCameraPos, characterCount, &clientRect, 0);

					BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDc, 0, 0, SRCCOPY);

					DeleteObject(hBm);
					DeleteDC(memDc);
				}

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

	Mesh mesh;
	bool meshLoaded = mesh.LoadFromFile("..\\Assets\\Models\\monkey2.obj");
	_ASSERT_EXPR(meshLoaded, TEXT("Failed to load mesh"));

	mesh.SetPosition({ 0, 0, 2 });
	pSceneRenderer->AddObjectToScene(&mesh);

	mainCamera.SetPosition({ 0, 0, 5 });
	mainCamera.LookAt({ 0, 0, 0 });

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

			// Update objects here
			UpdateCamera(hwnd, &mainCamera);

			static float inc = 0.0f;
			inc += 0.1f * g_Timer.GetLockedTime();
			mesh.SetRotation({ inc / 2.5f, inc / 2, inc });

			// Start record of the most recent render
			g_Timer.Reset();

			Render(hwnd);
			InvalidateRect(hwnd, nullptr, false);

			int currentFps = (int)(1000.0f / (fpsCounter.Elapsed() * 1000) + .5f);
			UpdateTitle(hwnd, currentFps);

			fpsCounter.Reset();
			fpsLimiter.Reset();
		}
	}
}

void Render(HWND hwnd)
{
	if (!pSrgc) return;
	pSrgc->frameBuffer->Clear({ 0, 0, 0 });
	pSceneRenderer->RenderScene(*pSrgc);
}

void UpdateCamera(HWND hwnd, Camera* camera)
{
	if (GetForegroundWindow() != hwnd) return;

	const Vert3df& cameraPos = camera->GetPosition();
	const USHORT keyDownFlag = 1 << 15;
	const float speed = 2.0f * g_Timer.GetLockedTime();

	// Forward
	if (GetKeyState('W') & keyDownFlag)
	{
		//camera->SetPosition({ cameraPos.x, cameraPos.y, cameraPos.z + -speed });
		camera->Move(MoveDirection::Forward, speed);
	}

	// Backward
	if (GetKeyState('S') & keyDownFlag)
	{
		camera->Move(MoveDirection::Backward, speed);
	}

	// Left
	if (GetKeyState('A') & keyDownFlag)
	{
		camera->Move(MoveDirection::Left, speed);
	}

	// Right
	if (GetKeyState('D') & keyDownFlag)
	{
		camera->Move(MoveDirection::Right, speed);
	}

	// Up
	if (GetKeyState(VK_LSHIFT) & keyDownFlag)
	{
		camera->Move(MoveDirection::Up, speed);
	}

	// Down
	if (GetKeyState(VK_LCONTROL) & keyDownFlag)
	{
		camera->Move(MoveDirection::Down, speed);
	}

	// Handle mouse movement
	/* TODO
	RECT rect;
	GetClientRect(hwnd, &rect);

	POINT screenCenter = { rect.right / 2, rect.bottom / 2 };
	POINT cursorPos = GetMouseCoordinates(hwnd);

	bool mouseHasMoved = cursorPos.x != screenCenter.x || cursorPos.y != screenCenter.y;
	if (mouseHasMoved)
	{
		Vec2df vecDirection(cursorPos.x - screenCenter.x, cursorPos.y - screenCenter.y);


		// Re-center mouse position
		CenterCursorPosition(hwnd);
	}
	*/
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

void CenterCursorPosition(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	POINT point = { rect.right / 2, rect.bottom / 2 };

	ClientToScreen(hwnd, &point);
	SetCursorPos(point.x, point.y);
}