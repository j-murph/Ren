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

	const Mat4x4f& wv = worldMatrix * viewMatrix;
	const Mat4x4f& wvp = wv * projectionMatrix;

	const float screenWidth = static_cast<float>(gfx.frameBuffer->GetWidth());
	const float screenHeight = static_cast<float>(gfx.frameBuffer->GetHeight());

	Tri2di rasterTri;
	for (Tri3df& tri : mesh.GetTriangles())
	{
		/*if (gfx.options.enableBackfaceCulling && 0)
		{
			Vec3df vecLookDirection = { vm(2, 0), vm(2, 1), vm(2, 2) };
			Vec3df vec = Vec3df(vm.GetTranslation()) + vecLookDirection;

			if (vec.Dot(tri.GetNormal()) < 0.f)
				continue;
		}*/

		DebugDrawNormal(tri, worldMatrix, gfx);

		Vert4df p1 = Vert4df(tri.p1, 1), p2 = Vert4df(tri.p2, 1), p3 = Vert4df(tri.p3, 1);

		// Get raster space coordinates
		Vert4df p1c = wvp * p1, p2c = wvp * p2, p3c = wvp * p3;
		
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

void SceneRenderer::DebugDrawNormal(const Tri3df& tri, const Mat4x4f& worldMatrix, const SRGraphicsContext& gfx)
{
	Vert4df p1 = Vert4df(tri.p1, 1), p2 = Vert4df(tri.p2, 1), p3 = Vert4df(tri.p3, 1);
	Vert4df p1m = worldMatrix * p1, p2m = worldMatrix * p2, p3m = worldMatrix * p3;

	Tri3df worldTri = { p1m.x, p1m.y, p1m.z, p2m.x, p2m.y, p2m.z, p3m.x, p3m.y, p3m.z };

	Vert3df vertCenterPoint = worldTri.GetCenter();
	Vert3df normalOffset = worldTri.GetNormal() * .1f;
	Vert3df vertEndPoint = vertCenterPoint + normalOffset;

	DebugDrawLine(vertCenterPoint, vertEndPoint, gfx);
}

void SceneRenderer::DebugDrawLine(const Vert3df& p1, const Vert3df& p2, const SRGraphicsContext& gfx)
{
	const Mat4x4f& viewMatrix = camera->GetViewMatrix();
	const Mat4x4f& projectionMatrix = camera->GetProjectionMatrix();
	const Mat4x4f& vp = viewMatrix * projectionMatrix;

	const float screenWidth = static_cast<float>(gfx.frameBuffer->GetWidth());
	const float screenHeight = static_cast<float>(gfx.frameBuffer->GetHeight());

	Vert4df p1c = vp * Vert4df(p1, 1), p2c = vp * Vert4df(p2, 1);

	p1c.DivideByW();
	p2c.DivideByW();

	Vert2di rasterPoints[2];
	rasterPoints[0].x = static_cast<int>((p1c.x + 1.0f) / 2.0f * screenWidth);
	rasterPoints[0].y = static_cast<int>((p1c.y + 1.0f) / 2.0f * screenHeight);

	rasterPoints[1].x = static_cast<int>((p2c.x + 1.0f) / 2.0f * screenWidth);
	rasterPoints[1].y = static_cast<int>((p2c.y + 1.0f) / 2.0f * screenHeight);

	rasterizer->DrawLine(rasterPoints[0], rasterPoints[1], gfx);
}