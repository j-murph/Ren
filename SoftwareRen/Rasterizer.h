#pragma once
#include "GraphicsContext.h"

enum RasterizerMode
{
	Point = 1,
	Wireframe = 2,
	Filled = 3
};

class Rasterizer
{
private:
	RasterizerMode mode;

	void DrawPointTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx);
	void DrawWireframeTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx);
	void DrawFilledTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx);

	void (Rasterizer::*CurrentModeDrawTriangle)(const Tri2di&, const SRGraphicsContext&);

public:
	Rasterizer();

	void SetRasterizerMode(RasterizerMode mode);
	RasterizerMode GetRasterizerMode();

	void DrawTriangle(const Tri2di& triangle, const SRGraphicsContext& gfx);

	void DrawLine(const Vert2di& p1, const Vert2di& p2, const SRGraphicsContext& gfx);
	void DrawLine(int p1x, int p1y, int p2x, int p2y, const SRGraphicsContext& gfx);

	void DrawPoint(const Vert2di& p1, const SRGraphicsContext& gfx);
	void DrawPoint(int p1x, int p1y, const SRGraphicsContext& gfx);
};
