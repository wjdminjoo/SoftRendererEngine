
#include "Precompiled.h"
#include "Camera.h"

Matrix4x4 Camera::GetViewMatrix() const
{

	Matrix4x4 PositionMat(Vector4::UnitX,
		Vector4::UnitY,
		Vector4::UnitZ,
		Vector4(-cameraTrans.GetPosition(), true));

	return PositionMat;
}
