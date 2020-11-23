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
	const Mat4x4f& vp = viewMatrix * projectionMatrix;

	const Vert3df& cameraPos = camera->GetPosition();

	const float screenWidth = static_cast<float>(gfx.frameBuffer->GetWidth());
	const float screenHeight = static_cast<float>(gfx.frameBuffer->GetHeight());

	Tri2di rasterTri;
	for (Tri3df& tri : mesh.GetTriangles())
	{
		Vert4df p1c = worldMatrix * Vert4df(tri.p1, 1),
				p2c = worldMatrix * Vert4df(tri.p2, 1),
				p3c = worldMatrix * Vert4df(tri.p3, 1);

		if (gfx.options.cullBackfaces)
		{
			Tri3df worldTri = { p1c.x, p1c.y, p1c.z, p2c.x, p2c.y, p2c.z, p3c.x, p3c.y, p3c.z };
			Vec3df vecCamToTri = worldTri.GetCenter() - cameraPos;

			if (vecCamToTri.Dot(worldTri.GetNormal()) >= 0.f)
			{
				continue;
			}
		}

		//DebugDrawNormal(tri, worldMatrix, gfx);

		p1c = vp * p1c;
		p2c = vp * p2c;
		p3c = vp * p3c;
		
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
	Vert4df p1m = worldMatrix * Vert4df(tri.p1, 1),
			p2m = worldMatrix * Vert4df(tri.p2, 1),
			p3m = worldMatrix * Vert4df(tri.p3, 1);

	Tri3df worldTri = { p1m.x, p1m.y, p1m.z, p2m.x, p2m.y, p2m.z, p3m.x, p3m.y, p3m.z };

	Vert3df vertCenterPoint = worldTri.GetCenter();
	Vert3df normalOffset = worldTri.GetNormal() * .02f;
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