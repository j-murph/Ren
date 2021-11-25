#pragma once
#include "stdafx.h"
#include "Rasterizer.h"

void RenderRotTri(HWND hwnd, Rasterizer* raster, SRGraphicsContext* pSrgc)
{
	POINT p = GetMouseCoordinates(hwnd);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	int x = p.x;
	int y = p.y;

	static float theta = 0.0f;
	theta += 1.f * g_Timer.GetLockedTime();

	Vert2di verts[3] = { { 0, 100 },{ -200, 0 },{ 600, 0 } };
	for (int i = 0; i < 3; i++)
	{
		Vert2di& v = verts[i];
		float t = (float)std::atan2(v.x, v.y);
		float l = (float)std::sqrt(v.x * v.x + v.y * v.y);

		v.x = (int)(std::cos(t + theta) * l + x);
		v.y = (int)(std::sin(t + theta) * l + y);
	}

	// Calculate center point of triangle
	Vert2di edgeCenterPoints;
	edgeCenterPoints.x = Lerp(verts[0].x, verts[1].x, .5f);
	edgeCenterPoints.y = Lerp(verts[0].y, verts[1].y, .5f);

	Vert2di centerPoint(Lerp<int>(verts[2].x, edgeCenterPoints.x, .666666f), Lerp<int>(verts[2].y, edgeCenterPoints.y, .666666f));
	Vec2di translateVector(x - centerPoint.x, y - centerPoint.y);

	for (int i = 0; i < 3; i++)
	{
		verts[i].x += translateVector.x;
		verts[i].y += translateVector.y;
	}

	raster->SetRasterizerMode(RasterizerMode::Wireframe);
	raster->DrawTriangle(Tri2di(verts[0].x, verts[0].y, verts[1].x, verts[1].y, verts[2].x, verts[2].y), *pSrgc);
}