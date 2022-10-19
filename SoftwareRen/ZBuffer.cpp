#include "stdafx.h"
#include <float.h>
#include "ZBuffer.h"

ZBuffer::ZBuffer() : width(0), height(0)
{
}

ZBuffer::~ZBuffer()
{
}

void ZBuffer::Init(int width, int height)
{
	SetSize(width, height);
}

void ZBuffer::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;

	buffer.reset(new float[width * height]);
}

void ZBuffer::Clear(float value)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			buffer[x + y * width] = FLT_MAX;
		}
	}
}
