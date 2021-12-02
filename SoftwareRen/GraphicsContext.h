#pragma once
#include <atomic>
#include "Framebuffer.h"
#include "ZBuffer.h"

struct SRGraphicsContext
{
	std::unique_ptr<Framebuffer> frameBuffer;
	std::unique_ptr<ZBuffer> zBuffer;
	HWND targetWindow;

	struct
	{
		std::atomic<bool> cullBackfaces = true;
		std::atomic<bool> drawNormals = true;
	} options;

	SRGraphicsContext(HWND targetWindow) : frameBuffer(std::make_unique<Framebuffer>()),
		zBuffer(std::make_unique<ZBuffer>()), targetWindow(targetWindow)
	{
	}

	HWND GetTargetWindow()
	{
		return targetWindow;
	}
};