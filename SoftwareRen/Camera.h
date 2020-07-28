#pragma once
#include "Math.h"

class Camera
{
private:
	Mat4x4f viewMatrix;
	Mat4x4f projectionMatrix;

	Vec3df position;
	Vec3df lookAt;

	float fov;
	float nearPlane;
	float farPlane;

	int screenWidth;
	int screenHeight;

	void UpdateProjectionMatrix();

public:
	Camera();

	const Mat4x4f& GetViewMatrix();
	const Mat4x4f& GetProjectionMatrix();

	void SetPosition(const Vert3df& position);
	Vert3df GetPosition();

	void SetRotationX(float angle);
	float GetRotationX();

	void SetRotationY(float angle);
	float GetRotationY();

	void SetRotationZ(float angle);
	float GetRotationZ();

	void SetFOV(float fov);
	float GetFOV();

	void SetScreenWidth(int width);
	int GetScreenWidth();

	void SetScreenHeight(int height);
	int GetScreenHeight();

	void SetNearPlane(float nearPlane);
	float GetNearPlane();

	void SetFarPlane(float farPlane);
	float GetFarPlane();

	Vec3df GetLookDirection() const;
	void LookAt(const Vert3df& at, Vec3df up = Vec3df(0.0f, 1.0f, 0.0f));
};
