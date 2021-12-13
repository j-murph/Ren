#pragma once
#include "resource.h"
#include "GraphicsContext.h"
#include "Camera.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitWindow(HINSTANCE hInstance, int nCmdShow);
std::unique_ptr<SRGraphicsContext> InitGraphics(HWND hwnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int	MessageLoop(HWND hwnd, HINSTANCE hInstance);
void UpdateCamera(HWND hwnd, Camera* camera);
void UpdateTitle(HWND hwnd, int currentFps);
void Render();
void HandlePaint(HWND hwnd);
void CenterCursorPosition(HWND hwnd);
void ToggleViewMode();
void HandleWindowResize(LPARAM lParam, WPARAM wParam, HWND hwnd);
void HandleKeyDown(WPARAM wParam);
