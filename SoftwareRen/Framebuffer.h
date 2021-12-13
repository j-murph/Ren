#pragma once
#include "Color.h"

class FrameBuffer
{
private:
	HBITMAP frontBufferBitmap; // Raw pixels are drawn to this bitmap

	HWND targetWindow;
	HDC memDc;

	void* pixels;

	int width;
	int height;

public:
	FrameBuffer();
	~FrameBuffer();

	bool Init(HWND targetWindow, int width, int height);

	bool SetSize(int width, int height);

	void Clear(const Color& color);

	inline void PutPixel(const int& x, const int& y, const Color& color);

	HDC BeginDraw(HDC hdc);
	void EndDraw(HDC hdc);

	void Free();

	int GetWidth() const;
	int GetHeight() const;
};

void FrameBuffer::PutPixel(const int& x, const int& y, const Color& color)
{
	if (x < 0 || y < 0) return;
	if (x >= width || y >= height) return;

	DWORD value = (color.b) | (color.g << 8) | (color.r << 16);
	DWORD* dwPixels = static_cast<DWORD*>(pixels);
	dwPixels[x + y * width] = value;
}