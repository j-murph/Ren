#include "stdafx.h"
#include "Mesh.h"
#include <string>
#include <fstream>

RenderObjectType Mesh::GetType()
{
	return RenderObjectType::Mesh;
}

std::vector<Tri3df>& Mesh::GetTriangles()
{
	return triangles;
}

bool Mesh::LoadFromFile(const std::string& filename)
{
	return true;
	std::fstream file(filename.c_str(), std::fstream::in);
	if (!file.is_open()) return false;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0) continue;
		if (line[0] == '#') continue; // Comment, ignore
		std::string a1;
	}

	return false;
}
