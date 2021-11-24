#pragma once
#include "Framebuffer.h"
#include "ZBuffer.h"

struct SRGraphicsContext
{
	std::unique_ptr<Framebuffer> frameBuffer;
	std::unique_ptr<ZBuffer> zBuffer;
	HWND targetWindow;

	struct
	{
		bool cullBackfaces;
	} options;

	SRGraphicsContext(HWND targetWindow) : frameBuffer(std::make_unique<Framebuffer>()),
		zBuffer(std::make_unique<ZBuffer>()), targetWindow(targetWindow)
	{
		options.cullBackfaces = true;
	}

	HWND GetTargetWindow()
	{
		return targetWindow;
	}
};