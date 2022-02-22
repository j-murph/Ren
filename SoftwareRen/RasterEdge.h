#pragma once
#include "Math.h"

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
		length = (int)direction.Length();
		direction /= (float)length;
		currentDistance = 0;
	}

	bool Next(Vert2di& out)
	{
		if (currentDistance > length)
			return false;

		out.x = start.x + (int)(direction.x * currentDistance);
		out.y = start.y + (int)(direction.y * currentDistance);

		currentDistance++;

		return true;
	}
};
