#include "stdafx.h"
#include "Camera.h"

Camera::Camera() : screenWidth(1), screenHeight(1)
{
	viewMatrix.Identity();
	SetNearPlane(0.1f);
	SetFarPlane(10000.0f);
	SetFOV(DEG_TO_RAD(70.0f));
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
	viewMatrix.SetTranslation(position);
}

const Vert3df Camera::GetPosition()
{
	return viewMatrix.GetTranslation();
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

void Camera::SetScreenWidth(int width)
{
	screenWidth = width;
	UpdateProjectionMatrix();
}

int Camera::GetScreenWidth()
{
	return screenWidth;
}

void Camera::SetScreenHeight(int height)
{
	screenHeight = height;
	UpdateProjectionMatrix();
}

int Camera::GetScreenHeight()
{
	return screenHeight;
}

void Camera::SetNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
}

float Camera::GetNearPlane()
{
	return nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	this->farPlane = farPlane;
}

float Camera::GetFarPlane()
{
	return farPlane;
}

void Camera::LookAt(const Vert3df& at, Vec3df up)
{
	Vert3df from(viewMatrix(3, 0), viewMatrix(3, 1), viewMatrix(3, 2));

	auto zAxis = (at - from).Normalize();
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
	viewMatrix(3, 0) = -xAxis.Dot((Vec3df)from);
	viewMatrix(3, 1) = -yAxis.Dot((Vec3df)from);
	viewMatrix(3, 2) = -zAxis.Dot((Vec3df)from);
	viewMatrix(3, 3) = 1.0f;
}

void Camera::UpdateProjectionMatrix()
{
	const float aspectRatio = screenWidth / (float)screenHeight;

	float tanHalfFOV = std::tan(fov / 2.0f);
	float zRange = nearPlane - farPlane;

	projectionMatrix.Zero();
	projectionMatrix(0, 0) = 1.0f / (tanHalfFOV * aspectRatio);
	projectionMatrix(1, 1) = 1.0f / tanHalfFOV;
	projectionMatrix(2, 2) = (-nearPlane - farPlane) / zRange;
	projectionMatrix(2, 3) = 1; // Left handed, invert for right handed
	projectionMatrix(3, 2) = (2 * nearPlane * farPlane) / zRange;
}