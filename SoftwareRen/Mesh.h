#pragma once
#include <vector>
#include "RenderObject.h"

class Mesh : public RenderObject
{
private:
	std::vector<Tri3df> triangles;

public:
	RenderObjectType GetType() override;

	std::vector<Tri3df>& GetTriangles();

	bool LoadFromFile(const std::string& filename);

	void CreateCube(float size, const Vert3df& origin = { 0, 0, 0 });
};
