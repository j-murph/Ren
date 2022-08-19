#include "stdafx.h"
#include "Camera.h"

Camera::Camera() : viewportWidth(1), viewportHeight(1), position(0, 0, 1), lookAt(0, 0, 0), up(0, 1, 0)
{
	viewMatrix.Identity();
	SetNearPlane(0.1f);
	SetFarPlane(10000.0f);
	SetFOV(Deg2Radf(70.0f));
}

const Mat4x4f& Camera::GetViewMatrix()
{
	return viewMatrix;
}

const Mat4x4f& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::SetPosition(const Vert3df& position)
{
	Vec3df diff = this->position - position;
	this->position = position;
	LookAt(lookAt - diff);
}

const Vert3df& Camera::GetPosition()
{
	return position;
}

void Camera::SetRotationX(float angle)
{
	viewMatrix.SetRotationX(angle);
}

float Camera::GetRotationX()
{
	return 0.0f;
}

void Camera::SetRotationY(float angle)
{
	viewMatrix.SetRotationY(angle);
}

float Camera::GetRotationY()
{
	return 0.0f;
}

void Camera::SetRotationZ(float angle)
{
	viewMatrix.SetRotationZ(angle);
}

float Camera::GetRotationZ()
{
	return 0.0f;
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	UpdateProjectionMatrix();
}

float Camera::GetFOV()
{
	return fov;
}

void Camera::SetViewportWidth(int width)
{
	viewportWidth = width;
	UpdateProjectionMatrix();
}

int Camera::GetViewportWidth()
{
	return viewportWidth;
}

void Camera::SetViewportHeight(int height)
{
	viewportHeight = height;
	UpdateProjectionMatrix();
}

int Camera::GetViewportHeight()
{
	return viewportHeight;
}

void Camera::SetNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
	UpdateProjectionMatrix();
}

float Camera::GetNearPlane()
{
	return nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	this->farPlane = farPlane;
	UpdateProjectionMatrix();
}

float Camera::GetFarPlane()
{
	return farPlane;
}

Vec3df Camera::GetLookDirection() const
{
	return
	{
		viewMatrix.a[2][0],
		viewMatrix.a[2][1],
		viewMatrix.a[2][2]
	};
}

void Camera::LookAt(const Vert3df& at)
{
	lookAt = at;

	auto zAxis = (lookAt - position).Normalize();
	auto xAxis = up.Cross(zAxis).Normalize();
	auto yAxis = zAxis.Cross(xAxis);

	viewMatrix(0, 0) = xAxis.x;
	viewMatrix(0, 1) = yAxis.x;
	viewMatrix(0, 2) = zAxis.x;
	viewMatrix(0, 3) = 0.0f;
	viewMatrix(1, 0) = xAxis.y;
	viewMatrix(1, 1) = yAxis.y;
	viewMatrix(1, 2) = zAxis.y;
	viewMatrix(1, 3) = 0.0f;
	viewMatrix(2, 0) = xAxis.z;
	viewMatrix(2, 1) = yAxis.z;
	viewMatrix(2, 2) = zAxis.z;
	viewMatrix(2, 3) = 0.0f;
	viewMatrix(3, 0) = -xAxis.Dot(position);
	viewMatrix(3, 1) = -yAxis.Dot(position);
	viewMatrix(3, 2) = -zAxis.Dot(position);
	viewMatrix(3, 3) = 1.0f;
}

void Camera::UpdateProjectionMatrix()
{
	const float aspectRatio = viewportWidth / (float)viewportHeight;
	const float tanHalfFOV = std::tan(fov / 2.0f);
	const float zRange = nearPlane - farPlane;

	projectionMatrix.Zero();
	projectionMatrix(0, 0) = 1.0f / (tanHalfFOV * aspectRatio);
	projectionMatrix(1, 1) = 1.0f / tanHalfFOV;
	projectionMatrix(2, 2) = (-nearPlane - farPlane) / zRange;
	projectionMatrix(2, 3) = 1; // Left handed, invert for right handed
	projectionMatrix(3, 2) = (2 * nearPlane * farPlane) / zRange;
}

void Camera::Move(MoveDirection direction, float units)
{
	const auto zAxis = (lookAt - position).Normalize();
	const auto xAxis = up.Cross(zAxis).Normalize();
	const auto yAxis = zAxis.Cross(xAxis);

	switch (direction)
	{
	case MoveDirection::FORWARD:
		SetPosition(position + zAxis * units);
		break;
	case MoveDirection::BACKWARD:
		SetPosition(position - zAxis * units);
		break;
	case MoveDirection::LEFT:
		SetPosition(position - xAxis * units);
		break;
	case MoveDirection::RIGHT:
		SetPosition(position + xAxis * units);
		break;
	case MoveDirection::UP:
		SetPosition(position + up * units);
		break;
	case MoveDirection::DOWN:
		SetPosition(position - up * units);
		break;
	}
}