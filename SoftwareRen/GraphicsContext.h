#pragma once
#include "Framebuffer.h"
#include "ZBuffer.h"

struct SRGraphicsContext
{
	Framebuffer* frameBuffer;
	ZBuffer* zBuffer;
	HWND targetWindow;

	SRGraphicsContext(HWND targetWindow) : frameBuffer(new Framebuffer),
		zBuffer(new ZBuffer), targetWindow(targetWindow)
	{
	}

	~SRGraphicsContext()
	{
		SAFE_FREE(frameBuffer);
		SAFE_FREE(zBuffer);

	}

	const HWND& getTargetWindow()
	{
		return targetWindow;
	}
};