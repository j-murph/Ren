#pragma once
#include "Framebuffer.h"
#include "ZBuffer.h"

struct SRGraphicsContext
{
	Framebuffer* frameBuffer;
	ZBuffer* zBuffer;
	HWND targetWindow;

	struct
	{
		bool cullBackfaces;
	} options;

	SRGraphicsContext(HWND targetWindow) : frameBuffer(new Framebuffer),
		zBuffer(new ZBuffer), targetWindow(targetWindow)
	{
		options.cullBackfaces = true;
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