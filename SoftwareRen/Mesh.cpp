#include "stdafx.h"
#include "Mesh.h"
#include "Tokenizer.h"

#define OBJ_COMMENT_CHAR '#'
#define OBJ_WHITESPACE_CHAR ' '
#define OBJ_VERTEX_STR "v"
#define OBJ_FACE_STR "f"
#define OBJ_FACE_SEPARATOR_CHAR '/'

RenderObjectType Mesh::GetType()
{
	return RenderObjectType::MESH;
}

std::vector<Tri3df>& Mesh::GetTriangles()
{
	return triangles;
}

bool Mesh::LoadFromFile(const std::string& filename)
{
	// .obj supported for now
	std::fstream file(filename.c_str(), std::fstream::in);
	if (!file.is_open()) return false;

	std::vector<Vert3df> vertices;

	Tokenizer<char> tkn(true);
	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0 || line[0] == OBJ_COMMENT_CHAR) continue;

		tkn.TokenizeString(line, OBJ_WHITESPACE_CHAR);

		try
		{
			const std::string& spec = tkn[0];

			if (spec == OBJ_VERTEX_STR)
			{
				vertices.emplace_back(
					tkn.ParseToken<float>(1),
					tkn.ParseToken<float>(2),
					tkn.ParseToken<float>(3));
			}
			else if (spec == OBJ_FACE_STR)
			{
				Tokenizer<char> faceTkn(true);

				Tri3df triTemp;
				for (int i = 0; i < 4; i++)
				{
					if (i >= tkn.GetTokens().size() - 1)
					{
						break;
					}

					faceTkn.TokenizeString(tkn[i + 1], OBJ_FACE_SEPARATOR_CHAR);

					const int vertIndex = faceTkn.ParseToken<int>(0);
					const Vert3df& vertex = vertices.at(vertIndex - 1);

					faceTkn.Clear();

					if (i == 0)
					{
						triTemp.p1 = vertex;
					}
					else if (i == 1)
					{
						triTemp.p2 = vertex;
					}
					else if (i == 2)
					{
						triTemp.p3 = vertex;
						triangles.push_back(triTemp);
					}
					else if (i == 3)
					{
						triTemp = Tri3df(triTemp.p1, triTemp.p3, vertex);
						triangles.push_back(triTemp);
					}
				}
			}

			tkn.Clear();
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	return true;
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
	triangles.push_back(Tri3df(topBackLeft, bottomBackLeft, topBackRight));
	triangles.push_back(Tri3df(topBackRight, bottomBackLeft, bottomBackRight));

	// Front face
	triangles.push_back(Tri3df(topFrontLeft, topFrontRight, bottomFrontLeft));
	triangles.push_back(Tri3df(topFrontRight, bottomFrontRight, bottomFrontLeft));

	// Left face
	triangles.push_back(Tri3df(topBackLeft, topFrontLeft, bottomFrontLeft));
	triangles.push_back(Tri3df(topBackLeft, bottomFrontLeft, bottomBackLeft));

	// Right face
	triangles.push_back(Tri3df(bottomFrontRight, topFrontRight, topBackRight));
	triangles.push_back(Tri3df(bottomFrontRight, topBackRight, bottomBackRight));

	// Top face
	triangles.push_back(Tri3df(topFrontLeft, topBackLeft, topBackRight));
	triangles.push_back(Tri3df(topFrontLeft, topBackRight, topFrontRight));

	// Bottom face
	triangles.push_back(Tri3df(bottomFrontLeft, bottomBackRight, bottomBackLeft));
	triangles.push_back(Tri3df(bottomFrontLeft, bottomFrontRight, bottomBackRight));
}
