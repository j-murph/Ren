#pragma once
#include "Math.h"

enum class RenderObjectType
{
	MESH = 1,
	BILLBOARD = 2
};

class RenderObject
{
private:
	Vert3df position;
	
	Vec3df rotation; // TODO: Quaternion

public:
	RenderObject();
	virtual ~RenderObject() = default;

	virtual Mat4x4f GetWorldMatrix();
	virtual RenderObjectType GetType() = 0;

	void SetPosition(const Vert3df& pos);
	Vert3df& GetPosition();

	void SetRotation(const Vec3df& rotation);
	Vec3df& GetRotation();
};