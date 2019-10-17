
#include "Precompiled.h"
#include "Transform2D.h"

Matrix3x3 Transform2D::GetTRS() const
{
	Matrix3x3 translateMatrix(Vector3::UnitX, Vector3::UnitY, Vector3(Position, true));

	float Theta = Math::Deg2Rad(RotationInDegree);
	float sinTheta, cosTheta;
	Math::SinCos(&sinTheta, &cosTheta, RotationInDegree);

	Matrix3x3 rotateMatrix(Vector3(Vector2(cosTheta, sinTheta), false), Vector3(Vector2(-sinTheta, cosTheta), false), Vector3::UnitZ);

	Matrix3x3 scaleMatrix(Vector3::UnitX * Scale.X, Vector3::UnitY * Scale.Y, Vector3::UnitZ);

	return  translateMatrix * rotateMatrix * scaleMatrix;
}

void Transform2D::SetPosition(const Vector2 & InPosition)
{
	Position = InPosition;

}

void Transform2D::SetRotation(float InEulerRotationInDegree)
{
	RotationInDegree = InEulerRotationInDegree;
}

void Transform2D::SetScale(const Vector2 & InScale)
{
	Scale = InScale;
}

void Transform2D::AddPosition(const Vector2 & InPosition)
{
	Position += InPosition;
}

void Transform2D::AddRotation(float InRotationDegree)
{
	RotationInDegree += InRotationDegree;
}

void Transform2D::AddScale(const Vector2 & InScale)
{
	Scale += InScale;
}