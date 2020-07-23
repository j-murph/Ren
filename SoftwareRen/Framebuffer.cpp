#include "stdafx.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer() : hdc(nullptr), bitmap(nullptr), width(0), height(0)
{
}

Framebuffer::~Framebuffer()
{
	Free();
}

bool Framebuffer::Init(HDC hdc, int width, int height)
{
	this->hdc = hdc;
	return SetSize(width, height);
}

bool Framebuffer::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;

	if (bitmap)
	{
		Free();
	}

	ZeroMemory(&bmInfo, sizeof(bmInfo));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biWidth = width;
	bmInfo.bmiHeader.biHeight = height; // Negate this to flip the final image
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	bitmap = CreateDIBSection(hdc, &bmInfo, DIB_RGB_COLORS, &pixels, nullptr, 0);

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

void Framebuffer::Draw()
{
	HDC dc = CreateCompatibleDC(hdc);
	SelectObject(dc, bitmap);
	BitBlt(hdc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
	DeleteDC(dc);
}

void Framebuffer::Free()
{
	if (bitmap)
	{
		DeleteObject(bitmap);
		bitmap = nullptr;
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
