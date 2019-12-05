
#include "Precompiled.h"
#include "Camera.h"
#include "GameObject.h"

Matrix4x4 Camera::GetViewMatrix(GameObject& InTargetObject) const
{
	// � ��ü�� �� �� ������ �̿��ؼ�, ȸ�� ��� �����.

	Vector3 viewZ = (cameraTrans.Position - InTargetObject.GetTransform().Position).Normalize();
	Vector3 viewX = Vector3::UnitY.Cross(viewZ).Normalize();
	if (viewX.IsZero()) {
		viewX = Vector3::UnitX;
	}
	Vector3 viewY = viewZ.Cross(viewX).Normalize();
	Matrix4x4 virMat = Matrix4x4(Vector4(viewX, false), Vector4(viewY, false), Vector4(viewZ, false), Vector4::UnitW);
	Matrix4x4 virRot = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-cameraTrans.Position));
	return virMat * virRot;
}

Matrix4x4 Camera::GetProjectionMatrix(int InScreenSizeX, int InScreenSizeY) const
{
	// ���� ���. ���� ���� ������ 0~1
	// Y�� �������� 1�� ��Ⱦ�� ���.
	// ���̰�.
	// Open GL : -1 ~ 1
	// DirectX : 0 ~ 1

	static float invA = (float)InScreenSizeY / (float)InScreenSizeX;
	static float d = 1.f / tanf(Math::Deg2Rad(FOV) * 0.5f);


	float invNF = 1.f / (NearZ - FarZ);
	float k = FarZ * invNF;
	float l = FarZ * NearZ * invNF;
	return Matrix4x4(
		Vector4::UnitX * invA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.f),
		Vector4(0.f, 0.f, l, 0.f));
}
