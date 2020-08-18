#pragma once
#include "Color.h"

class Framebuffer
{
private:
	HBITMAP bitmap;
	BITMAPINFO bmInfo;

	HWND targetWindow;
	HDC targetWindowHdc;

	void* pixels;

	int width;
	int height;

public:
	Framebuffer();
	~Framebuffer();

	bool Init(HWND targetWindow, int width, int height);

	bool SetSize(int width, int height);

	void Clear(Color color);

	__forceinline void PutPixel(const int& x, const int& y, const Color& color)
	{
		if (x < 0 || y < 0) return;
		if (x >= width || y >= height) return;

		DWORD value = (color.b) | (color.g << 8) | (color.r << 16);
		DWORD* dwPixels = static_cast<DWORD*>(pixels);
		dwPixels[x + y * width] = value;
	}

	void Draw(HDC drawToHdc);

	void Free();

	int GetWidth() const;
	int GetHeight() const;
};