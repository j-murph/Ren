#include "stdafx.h"
#include "RenWindow.h"
#include "Win32Helpers.h"

RenWindow::RenWindow() :
	pRasterizer(std::make_unique<Rasterizer>()),
	pSceneRenderer(std::make_unique<SceneRenderer>(pRasterizer.get())),
	handleMouseInput(true)
{
}

bool RenWindow::Create(int width, int height, LPCTSTR title)
{
	if (!Window::Create(width, height, title))
		return false;

	auto hwnd = GetWindowHandle();

	pSrgc = std::make_unique<SRGraphicsContext>();

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (!pSrgc->frameBuffer->Init(hwnd, clientRect.right, clientRect.bottom))
	{
		pSrgc.reset();
		return false;
	}

	pSrgc->zBuffer->Init(clientRect.right, clientRect.bottom);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	UpdateTitle(0);

	return true;
}

void RenWindow::Tick()
{
	if (!assetsLoaded)
	{
		assetsLoaded = true;

		bool meshLoaded;
		const float loadTime = BenchmarkFunction([&]()
		{
			meshLoaded = objMesh.LoadFromFile("..\\Assets\\Models\\libertstatue.obj");
		});

		_ASSERT_EXPR(meshLoaded, TEXT("Failed to load mesh"));

		std::cout << "Loaded mesh in " << loadTime << " seconds" << std::endl;

		cubeMesh.CreateCube(.5);
		objMesh.SetPosition({ 0, 0, 0 });
		cubeMesh.SetPosition({ 0, -1, 0 });
		pSceneRenderer->AddObjectToScene(&cubeMesh);
		pSceneRenderer->AddObjectToScene(&objMesh);

		mainCamera.SetPosition({ 0, 0, 5 });
		mainCamera.LookAt({ 0, 0, 0 });

		pSceneRenderer->SetCamera(&mainCamera);
	}

	static Timer fpsLimiter;

	const float elapsed = fpsLimiter.Elapsed();
	const bool needsRender = elapsed >= FRAME_TICK_RATE;
	if (needsRender)
	{
		fpsLimiter.Reset();

		const int currentFps = static_cast<int>(round(1.0f / elapsed));
		UpdateTitle(currentFps);

		// Lock in the time since the last render
		g_Timer.Lock();

		// Update objects here
		UpdateCamera();

		static float inc = 0.0f;
		inc += 0.65f * g_Timer.GetLockedTime();
		cubeMesh.SetRotation({ inc / .8f, -inc, inc / 1.2f });

		// Start record of the most recent render
		g_Timer.Reset();

		Render();

		InvalidateRect(GetWindowHandle(), nullptr, false);
	}
}

LRESULT RenWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	break;
	case WM_CREATE:
		ShowCursor(false);
		CenterCursorPosition(GetWindowHandle());
		break;
	case WM_PAINT:
		HandlePaint();
		break;
	case WM_SIZE:
		HandleWindowResize(lParam, wParam);
		break;
	case WM_KEYDOWN:
		HandleKeyDown(wParam);
		break;
	default:
		return DefWindowProc(GetWindowHandle(), message, wParam, lParam);
	}
	return 0;
}

void RenWindow::Render()
{
	pSrgc->frameBuffer->Clear({ 0, 0, 0 });
	pSceneRenderer->RenderScene(*pSrgc);
}

void RenWindow::HandlePaint()
{
	auto hwnd = GetWindowHandle();

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC memDc = pSrgc->frameBuffer->BeginDraw(hdc);

	const Vert3df& cameraPos = mainCamera.GetPosition();
	const Vert3df& lookAt = mainCamera.GetLookDirection();

	wchar_t szCameraPos[255] = { 0 };
	int characterCount = swprintf_s(szCameraPos, 255, L"Camera Position: %f %f %f\nCamera Look Direction: %f %f %f",
		cameraPos.x, cameraPos.y, cameraPos.z, lookAt.x, lookAt.y, lookAt.z);

	SetTextColor(memDc, 0x00FFFFFF);
	SetBkMode(memDc, TRANSPARENT);
	DrawText(memDc, szCameraPos, characterCount, &clientRect, 0);

	pSrgc->frameBuffer->EndDraw(hdc);
	EndPaint(hwnd, &ps);
}

void RenWindow::HandleWindowResize(LPARAM lParam, WPARAM wParam)
{
	const WORD newWidth = LOWORD(lParam), newHeight = HIWORD(lParam);
	const bool dimensionsChanged = newWidth != mainCamera.GetViewportWidth() || newHeight != mainCamera.GetViewportHeight();
	const bool windowMinimized = wParam == SIZE_MINIMIZED;

	if (dimensionsChanged && !windowMinimized)
	{
		RECT clientRect;
		GetClientRect(GetWindowHandle(), &clientRect);

		bool resizeSuccess = pSrgc->frameBuffer->SetSize(clientRect.right, clientRect.bottom);
		_ASSERT(resizeSuccess);
		pSrgc->zBuffer->SetSize(clientRect.right, clientRect.bottom);

		mainCamera.SetViewportWidth(clientRect.right);
		mainCamera.SetViewportHeight(clientRect.bottom);
	}
}

void RenWindow::UpdateCamera()
{
	if (GetForegroundWindow() != GetWindowHandle()) return;

	const Vert3df& cameraPos = mainCamera.GetPosition();
	const float speed = 2.0f * g_Timer.GetLockedTime();

	auto keyDown = [&](int key)
	{
		const USHORT keyDownFlag = 1 << 15;
		return GetKeyState(key) & keyDownFlag;
	};

	// Forward
	if (keyDown('W'))
	{
		mainCamera.Move(MoveDirection::FORWARD, speed);
		
	}

	// Backward
	if (keyDown('S'))
	{
		mainCamera.Move(MoveDirection::BACKWARD, speed);
	}

	// Left
	if (keyDown('A'))
	{
		mainCamera.Move(MoveDirection::LEFT, speed);
	}

	// Right
	if (keyDown('D'))
	{
		mainCamera.Move(MoveDirection::RIGHT, speed);
	}

	// Up
	if (keyDown(VK_LSHIFT))
	{
		mainCamera.Move(MoveDirection::UP, speed);
	}

	// Down
	if (keyDown(VK_LCONTROL))
	{
		mainCamera.Move(MoveDirection::DOWN, speed);
	}

	// TODO: Handle mouse movement
	if (handleMouseInput)
	{
		HWND hwnd = GetWindowHandle();
		RECT rect;
		GetClientRect(hwnd, &rect);

		POINT screenCenter = GetWindowClientCenter(hwnd);
		POINT cursorPos = GetMouseCoordinates(GetWindowHandle());

		bool mouseHasMoved = cursorPos.x != screenCenter.x || cursorPos.y != screenCenter.y;
		if (mouseHasMoved)
		{
			Vec2df vecDirection{ static_cast<float>(cursorPos.x - screenCenter.x), static_cast<float>(cursorPos.y - screenCenter.y) };
			mainCamera.RotateVertical(-Deg2Rad(vecDirection.x) / 10.0f);
			CenterCursorPosition(hwnd);
		}
	}
}

void RenWindow::UpdateTitle(int currentFps)
{
	static Timer lastUpdate;

	// Only update at 1hz
	if (lastUpdate.Elapsed() >= 1.0f)
	{
		lastUpdate.Reset();

		WCHAR title[256] = { 0 };
		swprintf_s(title, 256, TEXT("%s - %i FPS"), TEXT("Ren"), currentFps);
		SetWindowText(GetWindowHandle(), title);
	}
}

void RenWindow::ToggleViewMode()
{
	RasterizerMode mode = pSceneRenderer->GetRasterizer()->GetRasterizerMode();
	mode = static_cast<RasterizerMode>(((int)mode + 1) % (int)RasterizerMode::MODE_COUNT);
	pSceneRenderer->GetRasterizer()->SetRasterizerMode(mode);
}

void RenWindow::HandleKeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		Destroy();
		break;
	case 'V':
		ToggleViewMode();
		break;
	case 'C':
		pSrgc->options.cullBackfaces = !pSrgc->options.cullBackfaces;
		break;
	case 'N':
		pSrgc->options.drawNormals = !pSrgc->options.drawNormals;
		break;
	case 'E':
		handleMouseInput = !handleMouseInput;
		ShowCursor(!handleMouseInput);
		if (handleMouseInput)
		{
			CenterCursorPosition(GetWindowHandle());
		}
		break;
	}
}