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

void Mesh::CreateCube(float size, const Vert3df& origin)
{
	const Vert3df
		topBackLeft      = { -size + origin.x,  size + origin.y,  size + origin.z },
		topBackRight     = {  size + origin.x,  size + origin.y,  size + origin.z },
		bottomBackLeft   = { -size + origin.x, -size + origin.y,  size + origin.z },
		bottomBackRight  = {  size + origin.x, -size + origin.y,  size + origin.z },
		topFrontLeft     = { -size + origin.x,  size + origin.y, -size + origin.z },
		topFrontRight    = {  size + origin.x,  size + origin.y, -size + origin.z },
		bottomFrontLeft  = { -size + origin.x, -size + origin.y, -size + origin.z },
		bottomFrontRight = {  size + origin.x, -size + origin.y, -size + origin.z };

	auto& triangles = GetTriangles();

	// Back face
	triangles.push_back(Tri3df(topBackLeft, topBackRight, bottomBackLeft));
	triangles.push_back(Tri3df(topBackRight, bottomBackLeft, bottomBackRight));

	// Front face
	triangles.push_back(Tri3df(topFrontLeft, topFrontRight, bottomFrontLeft));
	triangles.push_back(Tri3df(topFrontRight, bottomFrontLeft, bottomFrontRight));

	// Left face
	triangles.push_back(Tri3df(bottomFrontLeft, topFrontLeft, topBackLeft));
	triangles.push_back(Tri3df(bottomFrontLeft, bottomBackLeft, topBackLeft));

	// Right face
	triangles.push_back(Tri3df(bottomFrontRight, topFrontRight, topBackRight));
	triangles.push_back(Tri3df(bottomFrontRight, bottomBackRight, topBackRight));

	// Top face
	triangles.push_back(Tri3df(topFrontLeft, topBackLeft, topBackRight));
	triangles.push_back(Tri3df(topFrontLeft, topBackRight, topFrontRight));

	// Bottom face
	triangles.push_back(Tri3df(bottomFrontLeft, bottomBackLeft, bottomBackRight));
	triangles.push_back(Tri3df(bottomFrontLeft, bottomBackRight, bottomFrontRight));
}
