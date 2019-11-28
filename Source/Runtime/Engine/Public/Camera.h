#pragma once
#include "Transform.h"

class Camera
{
public:
	Camera() = default;

public:
	Transform& GetTransform() { return cameraTrans; }
	Matrix4x4 GetViewMatrix() const;

private:
	Transform cameraTrans;
	float FOV = 60.f;
	float NearZ = 5.5f;
	float FarZ = 1000.f;
};

