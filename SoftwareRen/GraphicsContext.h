#pragma once
#include <atomic>
#include "FrameBuffer.h"
#include "ZBuffer.h"

struct SRGraphicsContext
{
	std::unique_ptr<FrameBuffer> frameBuffer;
	std::unique_ptr<ZBuffer> zBuffer;

	struct
	{
		std::atomic<bool> cullBackfaces = true;
		std::atomic<bool> drawNormals = false;
	} options;

	SRGraphicsContext() : frameBuffer(std::make_unique<FrameBuffer>()),
		zBuffer(std::make_unique<ZBuffer>())
	{
	}
};