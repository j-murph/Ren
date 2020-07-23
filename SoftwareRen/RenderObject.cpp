#include "stdafx.h"
#include "RenderObject.h"

RenderObject::RenderObject() : position(0, 0, 0)
{
}

RenderObject::~RenderObject()
{
}

Mat4x4f RenderObject::GetWorldMatrix()
{
	static float t = PI;
	t += PI / 600;
	Mat4x4f rotMatrix;
	rotMatrix.Identity();
	rotMatrix.SetRotationY(t);

	Mat4x4f tranMatrix;
	tranMatrix.Identity();
	tranMatrix.SetTranslation(position);
	
	return rotMatrix * tranMatrix;
	return tranMatrix;
}

void RenderObject::SetPosition(const Vert3df& pos)
{
	position = pos;
}

Vert3df& RenderObject::GetPosition()
{
	return position;
}
