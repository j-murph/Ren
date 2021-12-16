#include "stdafx.h"
#include "Rasterizer.h"
#include "RasterEdge.h"

Rasterizer::Rasterizer()
{
	SetRasterizerMode(RasterizerMode::FILLED);
}

void Rasterizer::SetRasterizerMode(RasterizerMode mode)
{
	this->mode = mode;
	switch (this->mode)
	{
	case RasterizerMode::POINT:
		CurrentModeDrawTriangle = &Rasterizer::DrawPointTriangle;
		break;
	case RasterizerMode::WIRE_FRAME:
		CurrentModeDrawTriangle = &Rasterizer::DrawWireframeTriangle;
		break;
	case RasterizerMode::FILLED:
		CurrentModeDrawTriangle = &Rasterizer::DrawFilledTriangle;
		break;
	default:
		_ASSERT_EXPR(false, TEXT("Invalid rasterizer mode."));
	}
}

RasterizerMode Rasterizer::GetRasterizerMode()
{
	return mode;
}

void Rasterizer::DrawTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx)
{
	(this->*CurrentModeDrawTriangle)(triangle, gfx);
}

void Rasterizer::DrawLine(const Vert2di& p1, const Vert2di& p2, const SRGraphicsContext& gfx)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, gfx);
}

void Rasterizer::DrawLine(int p1x, int p1y, int p2x, int p2y, const SRGraphicsContext& gfx)
{
	Color col(255, 0, 255);

	// Bresenham's algorithm
	bool steep = abs(p1y - p2y) > abs(p1x - p2x);

	if (steep)
	{
		std::swap(p1x, p1y);
		std::swap(p2y, p2x);
	}

	if (p1x > p2x)
	{
		std::swap(p1x, p2x);
		std::swap(p1y, p2y);
	}

	int inc = p1y < p2y ? 1 : -1;

	int dy = abs(p2y - p1y);
	int dx = p2x - p1x;

	int error = dx / 2;

	int& y = p1y;

	for (int x = p1x; x <= p2x; x++)
	{
		if (steep)
			gfx.frameBuffer->PutPixel(y, x, col);
		else
			gfx.frameBuffer->PutPixel(x, y, col);

		error -= dy;

		if (error < 0)
		{
			y += inc;
			error += dx;
		}
	}
}

void Rasterizer::DrawPoint(const Vert2di& p1, const SRGraphicsContext& gfx)
{
	Color col(0, 255, 255);
	gfx.frameBuffer->PutPixel(p1.x, p1.y, col);
}

void Rasterizer::DrawPoint(int x, int y, const SRGraphicsContext& gfx)
{
	Color col(255, 0, 255);
	gfx.frameBuffer->PutPixel(x, y, col);
}

void Rasterizer::DrawPointTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx)
{
	Color col(255, 0, 255);
	gfx.frameBuffer->PutPixel(triangle.p1.x, triangle.p1.y, col);
	gfx.frameBuffer->PutPixel(triangle.p2.x, triangle.p2.y, col);
	gfx.frameBuffer->PutPixel(triangle.p3.x, triangle.p3.y, col);
}

void Rasterizer::DrawWireframeTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx)
{
	DrawLine(triangle.p1, triangle.p2, gfx);
	DrawLine(triangle.p2, triangle.p3, gfx);
	DrawLine(triangle.p3, triangle.p1, gfx);
}

void Rasterizer::DrawFilledTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx)
{
	auto top = &triangle.p1,
		 mid = &triangle.p2,
		 bot = &triangle.p3;

	if (top->y < mid->y)
	{
		std::swap(top, mid);
	}

	if (top->y < bot->y)
	{
		std::swap(top, bot);
	}

	if (mid->y < bot->y)
	{
		std::swap(mid, bot);
	}

	Vert2di pix1 = *top, pix2 = *mid;
	RasterEdge edge1(*top, *bot), edge2(*top, *mid);

	int y = top->y;
	while (y > bot->y)
	{
		while (pix1.y != y)
		{
			if (!edge1.Next(pix1))
			{
				break;
			}
		}

		while (pix2.y != y)
		{
			if (!edge2.Next(pix2))
			{
				pix2 = edge2.Reset(*mid, *bot);
				break;
			}
		}

		DrawScanline(pix1.x, pix2.x, y, gfx);

		y--;
	}
}

void Rasterizer::DrawScanline(int startX, int endX, int y, const SRGraphicsContext& gfx)
{
	if (startX > endX)
		std::swap(startX, endX);

	while (startX <= endX)
	{
		gfx.frameBuffer->PutPixel(startX, y, Color(255, 0, 0));
		startX++;
	}
}