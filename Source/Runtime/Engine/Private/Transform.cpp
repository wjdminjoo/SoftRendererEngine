
#include "Precompiled.h"
#include "Transform.h"

Matrix4x4 Transform::GetModelingMatrix() const
{
	Matrix4x4 TransMatrix(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(Position));
	Matrix4x4 RotationMatrix;
	Matrix4x4 ScaleMatix(Vector4::UnitX * Scale.X, Vector4::UnitY * Scale.Y, Vector4::UnitZ * Scale.Z, Vector4::UnitW);
	return TransMatrix * RotationMatrix * ScaleMatix;
}
