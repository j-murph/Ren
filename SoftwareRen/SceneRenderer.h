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

public:
	SceneRenderer(Rasterizer* raster);

	void AddObjectToScene(RenderObject* object);

	void RenderScene(const SRGraphicsContext& gfx);

	void SetCamera(Camera* camera);
	Camera* GetCamera();
};
