#pragma once

struct Color
{
	unsigned char r, g, b;

	Color() = default;

	Color(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};
