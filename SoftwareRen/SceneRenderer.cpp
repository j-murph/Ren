#include "stdafx.h"
#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(Rasterizer* raster) : rasterizer(raster), camera(nullptr)
{
}

void SceneRenderer::AddObjectToScene(RenderObject* object)
{
	renderObjects.push_back(object);
}

void SceneRenderer::RenderScene(const SRGraphicsContext& gfx)
{
	_ASSERT_EXPR(camera != nullptr, TEXT("Camera must be set."));

	for (RenderObject*& object : renderObjects)
	{
		switch (object->GetType())
		{
		case RenderObjectType::MESH:
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

Rasterizer* SceneRenderer::GetRasterizer()
{
	return rasterizer;
}

void SceneRenderer::DrawMesh(Mesh& mesh, const SRGraphicsContext& gfx)
{
	const Mat4x4f& worldMatrix = mesh.GetWorldMatrix();
	const Mat4x4f& pvMat = camera->GetProjectionMatrix() * camera->GetViewMatrix();

	const Vert3df& cameraPos = camera->GetPosition();

	const float halfViewportWidth = static_cast<float>(gfx.frameBuffer->GetWidth()) / 2.0f;
	const float halfViewportHeight = static_cast<float>(gfx.frameBuffer->GetHeight()) / 2.0f;

	Tri2di rasterTri;
	for (const Tri3df& tri : mesh.GetTriangles())
	{
		Vert4df p1c = worldMatrix * Vert4df(tri.p1, 1),
				p2c = worldMatrix * Vert4df(tri.p2, 1),
				p3c = worldMatrix * Vert4df(tri.p3, 1);

		if (gfx.options.cullBackfaces)
		{
			const Tri3df worldTri = { p1c.x, p1c.y, p1c.z, p2c.x, p2c.y, p2c.z, p3c.x, p3c.y, p3c.z };
			Vec3df vecCamToTri = worldTri.GetCenter() - cameraPos;

			if (vecCamToTri.Dot(worldTri.GetNormal()) >= 0.f)
			{
				continue;
			}
		}

		p1c = pvMat * p1c;
		p2c = pvMat * p2c;
		p3c = pvMat * p3c;
		
		p1c.DivideByW();
		p2c.DivideByW();
		p3c.DivideByW();

		if (p1c.w <= 0.0f || p2c.w <= 0.0f || p3c.w <= 0.0f) continue;

		// Omit polygons that don't entirely fit in the viewport. TODO: Clipping
		if (p1c.x < -1.0f || p1c.x > 1.0f || p1c.y < -1.0f || p1c.y > 1.0f) continue;
		if (p2c.x < -1.0f || p2c.x > 1.0f || p2c.y < -1.0f || p2c.y > 1.0f) continue;
		if (p3c.x < -1.0f || p3c.x > 1.0f || p3c.y < -1.0f || p3c.y > 1.0f) continue;

		rasterTri.p1.x = static_cast<int>((p1c.x + 1.0f) * halfViewportWidth);
		rasterTri.p1.y = static_cast<int>((p1c.y + 1.0f) * halfViewportHeight);
		//rasterTri.p1.z = p1c.z;

		rasterTri.p2.x = static_cast<int>((p2c.x + 1.0f) * halfViewportWidth);
		rasterTri.p2.y = static_cast<int>((p2c.y + 1.0f) * halfViewportHeight);
		//rasterTri.p2.z = p2c.z;

		rasterTri.p3.x = static_cast<int>((p3c.x + 1.0f) * halfViewportWidth);
		rasterTri.p3.y = static_cast<int>((p3c.y + 1.0f) * halfViewportHeight);
		//rasterTri.p3.z = p3c.z;

		rasterizer->DrawTriangle(rasterTri, gfx);

		if (gfx.options.drawNormals)
			DebugDrawNormal(tri, worldMatrix, gfx);
	}
}

void SceneRenderer::DebugDrawNormal(const Tri3df& tri, const Mat4x4f& worldMatrix, const SRGraphicsContext& gfx)
{
	const Vert4df p1m = worldMatrix * Vert4df(tri.p1, 1),
			      p2m = worldMatrix * Vert4df(tri.p2, 1),
			      p3m = worldMatrix * Vert4df(tri.p3, 1);

	const Tri3df worldTri = { p1m.x, p1m.y, p1m.z, p2m.x, p2m.y, p2m.z, p3m.x, p3m.y, p3m.z };

	const Vert3df vertCenterPoint = worldTri.GetCenter();
	const Vert3df normalOffset = worldTri.GetNormal() * .13f;
	const Vert3df vertEndPoint = vertCenterPoint + normalOffset;

	DebugDrawLine(vertCenterPoint, vertEndPoint, gfx);
}

void SceneRenderer::DebugDrawLine(const Vert3df& p1, const Vert3df& p2, const SRGraphicsContext& gfx)
{
	const Mat4x4f& pvMat = camera->GetProjectionMatrix() *camera->GetViewMatrix();

	const float halfViewportWidth = static_cast<float>(gfx.frameBuffer->GetWidth()) / 2.0f;
	const float halfViewportHeight = static_cast<float>(gfx.frameBuffer->GetHeight()) / 2.0f;

	Vert4df p1c = pvMat * Vert4df(p1, 1), p2c = pvMat * Vert4df(p2, 1);

	p1c.DivideByW();
	p2c.DivideByW();

	if (p1c.w <= 0.0f || p2c.w <= 0.0f) return;

	Vert2di rasterPoints[2] = 
	{
		{ static_cast<int>((p1c.x + 1.0f) * halfViewportWidth), static_cast<int>((p1c.y + 1.0f) * halfViewportHeight) },
		{ static_cast<int>((p2c.x + 1.0f) * halfViewportWidth), static_cast<int>((p2c.y + 1.0f) * halfViewportHeight) }
	};

	rasterizer->DrawLine(rasterPoints[0], rasterPoints[1], gfx);
}