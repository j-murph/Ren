#include "stdafx.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() : targetWindow(nullptr), memDc(nullptr),
	frontBufferBitmap(nullptr), width(0), height(0), pixels(nullptr)
{
}

FrameBuffer::~FrameBuffer()
{
	Free();
}

bool FrameBuffer::Init(HWND targetWindow, int width, int height)
{
	this->targetWindow = targetWindow;
	return SetSize(width, height);
}

bool FrameBuffer::SetSize(int width, int height)
{
	Free();

	this->width = width;
	this->height = height;

	BITMAPINFO bmInfo{};
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biWidth = width;
	bmInfo.bmiHeader.biHeight = height; // Negate this to flip the final image
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	HDC targetWindowHdc = GetDC(targetWindow);
	frontBufferBitmap = CreateDIBSection(targetWindowHdc, &bmInfo, DIB_RGB_COLORS, &pixels, nullptr, 0);
	ReleaseDC(targetWindow, targetWindowHdc);

	return frontBufferBitmap != nullptr;
}

void FrameBuffer::Clear(const Color& color)
{
	DWORD dwordColor = (color.b) | (color.g << 8) | (color.r << 16);
	DWORD* currentPixel = static_cast<DWORD*>(pixels);

	// TODO: Optimize this
	int iterationCount = width * height;
	while (iterationCount-- != 0)
	{
		*currentPixel++ = dwordColor;
	}
}

HDC FrameBuffer::BeginDraw(HDC hdc)
{
	_ASSERT_EXPR(memDc == nullptr, TEXT("EndDraw() must be called after calling BeginDraw()."));
	memDc = CreateCompatibleDC(hdc);
	SelectObject(memDc, frontBufferBitmap);
	return memDc;
}

void FrameBuffer::EndDraw(HDC hdc)
{
	_ASSERT_EXPR(memDc != nullptr, TEXT("EndDraw() called before calling BeginDraw()."));
	BitBlt(hdc, 0, 0, width, height, memDc, 0, 0, SRCCOPY);
	DeleteDC(memDc);
	memDc = nullptr;
}

void FrameBuffer::Free()
{
	if (frontBufferBitmap)
	{
		DeleteObject(frontBufferBitmap);
		frontBufferBitmap = nullptr;
	}
}

int FrameBuffer::GetWidth() const
{
	return width;
}

int FrameBuffer::GetHeight() const
{
	return height;
}
