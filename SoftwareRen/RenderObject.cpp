#include "stdafx.h"
#include "RenderObject.h"

RenderObject::RenderObject() : position(0, 0, 0), rotation(0, 0, 0)
{
}

RenderObject::~RenderObject()
{
}

Mat4x4f RenderObject::GetWorldMatrix()
{
	Mat4x4f rotXMat;
	rotXMat.Identity();
	rotXMat.SetRotationX(rotation.x);

	Mat4x4f rotYMat;
	rotYMat.Identity();
	rotYMat.SetRotationY(rotation.y);

	Mat4x4f rotZMat;
	rotZMat.Identity();
	rotZMat.SetRotationY(rotation.z);

	Mat4x4f tranMatrix;
	tranMatrix.Identity();
	tranMatrix.SetTranslation(position);
	
	return rotXMat * rotYMat * rotZMat * tranMatrix;
}

void RenderObject::SetPosition(const Vert3df& pos)
{
	position = pos;
}

Vert3df& RenderObject::GetPosition()
{
	return position;
}

void RenderObject::SetRotation(const Vec3df& rotation)
{
	this->rotation = rotation;
}

Vec3df& RenderObject::GetRotation()
{
	return rotation;
}
