#include "stdafx.h"
#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(Rasterizer* raster) : rasterizer(raster)
{
}

void SceneRenderer::AddObjectToScene(RenderObject* object)
{
	renderObjects.push_back(object);
}

void SceneRenderer::RenderScene(const SRGraphicsContext& gfx)
{
	_ASSERT_EXPR(camera != nullptr, TEXT("Camera must be set."));

	rasterizer->SetRasterizerMode(RasterizerMode::Wireframe);

	for (RenderObject*& object : renderObjects)
	{
		switch (object->GetType())
		{
		case RenderObjectType::Mesh:
			DrawMesh(*(Mesh*)object, gfx);
			break;
		}
	}
}

void SceneRenderer::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* SceneRenderer::GetCamera()
{
	return camera;
}

void SceneRenderer::DrawMesh(Mesh& mesh, const SRGraphicsContext& gfx)
{
	const Mat4x4f& worldMatrix = mesh.GetWorldMatrix();
	const Mat4x4f& viewMatrix = camera->GetViewMatrix();
	const Mat4x4f& projectionMatrix = camera->GetProjectionMatrix();
	const Mat4x4f& pvm = worldMatrix * viewMatrix * projectionMatrix;

	const float screenWidth = static_cast<float>(gfx.frameBuffer->GetWidth());
	const float screenHeight = static_cast<float>(gfx.frameBuffer->GetHeight());

	Tri2di rasterTri;
	for (Tri3df& tri : mesh.GetTriangles())
	{
		Vert4df p1 = Vert4df(tri.p1, 1), p2 = Vert4df(tri.p2, 1), p3 = Vert4df(tri.p3, 1);

		// Get raster space coordinates
		Vert4df p1c = pvm * p1, p2c = pvm * p2, p3c = pvm * p3;
		
		p1c.DivideByW();
		p2c.DivideByW();
		p3c.DivideByW();

		rasterTri.p1.x = static_cast<int>((p1c.x + 1.0f) / 2.0f * screenWidth);
		rasterTri.p1.y = static_cast<int>((p1c.y + 1.0f) / 2.0f * screenHeight);
		//rasterTri.p1.z = p1c.z;

		rasterTri.p2.x = static_cast<int>((p2c.x + 1.0f) / 2.0f * screenWidth);
		rasterTri.p2.y = static_cast<int>((p2c.y + 1.0f) / 2.0f * screenHeight);
		//rasterTri.p2.z = p2c.z;

		rasterTri.p3.x = static_cast<int>((p3c.x + 1.0f) / 2.0f * screenWidth);
		rasterTri.p3.y = static_cast<int>((p3c.y + 1.0f) / 2.0f * screenHeight);
		//rasterTri.p3.z = p3c.z;

		rasterizer->DrawTriangle(rasterTri, gfx);
	}
}