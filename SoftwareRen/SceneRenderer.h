#pragma once
#include <vector>
#include "Mesh.h"
#include "Rasterizer.h"
#include "RenderObject.h"
#include "Camera.h"

class SceneRenderer
{
private:
	std::vector <RenderObject*> renderObjects;
	
	Rasterizer* rasterizer;

	Camera* camera;

	void DrawMesh(Mesh& mesh, const SRGraphicsContext& gfx);

	void DebugDrawLine(const Vert3df& p1, const Vert3df& p2, const SRGraphicsContext& gfx);

	void DebugDrawNormal(const Tri3df& tri, const Mat4x4f& worldMatrix, const SRGraphicsContext& gfx);

public:
	SceneRenderer(Rasterizer* raster);

	void AddObjectToScene(RenderObject* object);

	void RenderScene(const SRGraphicsContext& gfx);

	void SetCamera(Camera* camera);
	Camera* GetCamera();
};
