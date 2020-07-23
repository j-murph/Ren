#include "stdafx.h"
#include <float.h>
#include "ZBuffer.h"

ZBuffer::ZBuffer() : buffer(nullptr), width(0), height(0)
{
}

ZBuffer::~ZBuffer()
{
	SAFE_FREE_ARRAY(buffer);
}

void ZBuffer::Init(int width, int height)
{
	SetSize(width, height);
}

void ZBuffer::SetSize(int width, int height)
{
	SAFE_FREE(buffer);

	this->width = width;
	this->height = height;

	buffer = new float[width * height];
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
