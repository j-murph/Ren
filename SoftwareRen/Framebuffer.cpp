#include "stdafx.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer() : targetWindow(nullptr), targetWindowHdc(nullptr), bitmap(nullptr), width(0), height(0)
{
}

Framebuffer::~Framebuffer()
{
	Free();
}

bool Framebuffer::Init(HWND targetWindow, int width, int height)
{
	this->targetWindow = targetWindow;
	return SetSize(width, height);
}

bool Framebuffer::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	
	Free();

	targetWindowHdc = GetDC(this->targetWindow);

	ZeroMemory(&bmInfo, sizeof(bmInfo));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biWidth = width;
	bmInfo.bmiHeader.biHeight = height; // Negate this to flip the final image
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	bitmap = CreateDIBSection(targetWindowHdc, &bmInfo, DIB_RGB_COLORS, &pixels, nullptr, 0);

	return bitmap != nullptr;
}

void Framebuffer::Clear(Color color)
{
	DWORD value = (color.b) | (color.g << 8) | (color.r << 16);
	DWORD* curPixel = static_cast<DWORD*>(pixels);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			*curPixel = value;
			curPixel++;
		}
	}
}

void Framebuffer::Draw(HDC hdc)
{
	HDC memDc = CreateCompatibleDC(hdc);
	SelectObject(memDc, bitmap);
	BitBlt(hdc, 0, 0, width, height, memDc, 0, 0, SRCCOPY);
	DeleteDC(memDc);
}

void Framebuffer::Free()
{
	if (bitmap)
	{
		DeleteObject(bitmap);
		bitmap = nullptr;
	}

	if (targetWindowHdc)
	{
		ReleaseDC(targetWindow, targetWindowHdc);
		targetWindowHdc = nullptr;
	}
}

int Framebuffer::GetWidth() const
{
	return width;
}

int Framebuffer::GetHeight() const
{
	return height;
}
