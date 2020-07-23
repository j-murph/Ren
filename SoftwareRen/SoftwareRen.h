#pragma once

#include "resource.h"
#include "GraphicsContext.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitWindow(HINSTANCE, int);
SRGraphicsContext* InitGraphics(HWND hwnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int	MessageLoop(HWND hwnd, HINSTANCE hInstance);
void UpdateTitle(HWND hwnd, int currentFps);
void Render(HWND hwnd);