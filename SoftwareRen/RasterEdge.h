#pragma once
#include "Math.h"

// TODO: Imeplement this similarily to Bresenham's algo, avoiding FP calculations
struct RasterEdge
{
	Vert2di start;
	Vec2df direction;
	int length;
	int currentDistance;

	RasterEdge(const Vert2di& start, const Vert2di& end)
	{
		Reset(start, end);
	}

	void Reset(const Vert2di& start, const Vert2di& end)
	{
		this->start = start;
		direction = Vec2df((float)end.x, (float)end.y) - Vec2df((float)start.x, (float)start.y);
		length = static_cast<int>(direction.Length());
		direction /= static_cast<float>(length);
		currentDistance = 0;
	}

	bool Next(Vert2di& out)
	{
		if (currentDistance > length)
			return false;

		out.x = start.x + static_cast<int>(direction.x * currentDistance);
		out.y = start.y + static_cast<int>(direction.y * currentDistance);

		currentDistance++;

		return true;
	}
};
