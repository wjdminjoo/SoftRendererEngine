#include "Precompiled.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Vector4 & inCol0, const Vector4 & inCol1, const Vector4 & inCol2, const Vector4 & inCol3)
{
	Cols[0] = inCol0;
	Cols[1] = inCol1;
	Cols[2] = inCol2;
	Cols[3] = inCol3;
}

Matrix4x4::Matrix4x4(float In00, float In01, float In02, float In03, float In10, float In11, float In12, float In13, float In20, float In21, float In22, float In23, float In30, float In31, float In32, float In33)
{
	Cols[0][0] = In00;
	Cols[0][1] = In01;
	Cols[0][2] = In02;
	Cols[0][3] = In03;

	Cols[1][0] = In10;
	Cols[1][1] = In11;
	Cols[1][2] = In12;
	Cols[1][3] = In13;

	Cols[2][0] = In20;
	Cols[2][1] = In21;
	Cols[2][2] = In22;
	Cols[2][3] = In23;

	Cols[3][0] = In30;
	Cols[3][1] = In31;
	Cols[3][2] = In32;
	Cols[3][3] = In33;
}