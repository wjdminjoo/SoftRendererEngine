
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "WindowsTimer.h"

void SoftRenderer::Initialize()
{
	// Init System Timer
	MilliSecFrequency = WindowsTimer::Init();
	if (MilliSecFrequency == 0)
		return;

	FrameCount = 0;
	ElapsedTime = 0;

	// Init RSI
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::Update()
{
	if (RSI == nullptr) return;
	
	static float moveSpeed = 100;
	static float rotSpeed = 180;
	static Vector2 currentPos = Vector2::Zero;
	static float currentAngle = 0.f;
	Vector2 deltaPos = Vector2(InputManager.GetXAxis(), 0.f) * moveSpeed * FrameSec;
	float deltaAngle = InputManager.GetYAxis() * rotSpeed * FrameSec;
	currentPos += deltaPos;
	currentAngle += deltaAngle;
	float rad = Math::Deg2Rad(currentAngle);

	Matrix4x4 tMat;
	Matrix4x4 rMat;
	Matrix4x4 sMat(Vector4::UnitX * 100, Vector4::UnitY * 100, Vector4::UnitZ * 100, Vector4::UnitW);
	Matrix4x4 mMat = tMat * rMat * sMat;

	Vector3 cameraPosition(Vector3(-100.f, 0.f, 100.f));
	Vector3 viewZUnit = (cameraPosition - Vector3::Zero).Normalize();
	Vector3 viewXUnit = Vector3::UnitY.Cross(viewZUnit).Normalize();
	if (viewXUnit.IsZero())
	{
		viewXUnit = Vector3::UnitX;
	}
	Vector3 viewYUnit = viewZUnit.Cross(viewXUnit).Normalize();
	Vector4 zAxis(viewZUnit, false);
	Vector4 xAxis(viewXUnit, false);
	Vector4 yAxis(viewYUnit, false);
	Matrix4x4 virMat(xAxis, yAxis, zAxis, Vector4::UnitW);

	Matrix4x4 vitMat;
	Matrix4x4 vMat = virMat * vitMat;
	Matrix4x4 pMat;
	Matrix4x4 finalMat = pMat * vMat * mMat;
	const int vertexCount = 24;



	VertexData v[vertexCount] = {
		// Front
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		// Left
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		// Right
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
		// Back
		VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		// Top
		VertexData(Vector3(0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
		VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
		VertexData(Vector3(0.5f, 0.5f, -0.5f)),
		// Bottom
		VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, 0.5f)),
		VertexData(Vector3(0.5f, -0.5f, -0.5f)),
		VertexData(Vector3(-0.5f, -0.5f, -0.5f))
	};

	for (int vi = 0; vi < vertexCount; vi++)
	{
		v[vi].Position = finalMat * v[vi].Position;
	}
	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;
	int i[indexCount] = {
	 0, 2, 1, 0, 3, 2,
	 4, 6, 5, 4, 7, 6,
	 8, 10, 9, 8, 11, 10,
	 12, 14, 13, 12, 15, 14,
	 16, 18, 17, 16, 19, 18,
	 20, 22, 21, 20, 23, 22
	};

	for (int t = 0; t < triangleCount; t++)
	{
		RSI->DrawLine2(ScreenPoint(v[i[t * 3]].Position.ToVector2()), ScreenPoint(v[i[t * 3 + 1]].Position.ToVector2()));
		RSI->DrawLine2(ScreenPoint(v[i[t * 3]].Position.ToVector2()), ScreenPoint(v[i[t * 3 + 2]].Position.ToVector2()));
		RSI->DrawLine2(ScreenPoint(v[i[t * 3 + 1]].Position.ToVector2()), ScreenPoint(v[i[t * 3 + 2]].Position.ToVector2()));
	}
}

void SoftRenderer::PreUpdate()
{
	StartFrameSec = WindowsTimer::CheckMilliSeconds();

	if (RSI == nullptr) return;

	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.5f));

	// Render Start
	static int gridSize = 10;
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenSize = DisplaySetting::Inst().GetSize();

	int startX = -Math::RoundToInt(screenSize.X * 0.5f);
	int endX = -startX;

	int startY = -Math::RoundToInt(screenSize.Y * 0.5f);
	int endY = -startY;

	ScreenPoint xAxisStart(startX, 0);
	ScreenPoint xAxisEnd(endX, 0);
	ScreenPoint yAxisStart(0, startY);
	ScreenPoint yAxisEnd(0, endY);

	for (int x = 0; x <= endX; x += gridSize)
	{
		RSI->DrawVerticalLine(x, gridColor);
		if (x > 0)
		{
			RSI->DrawVerticalLine(-x, gridColor);
		}
	}

	for (int y = 0; y <= endY; y += gridSize)
	{
		RSI->DrawHorizontalLine(y, gridColor);
		if (y > 0)
		{
			RSI->DrawHorizontalLine(-y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawHorizontalLine(0, LinearColor::Red);
	RSI->DrawVerticalLine(0, LinearColor::Green);
}

void SoftRenderer::PostUpdate()
{
	if (RSI != nullptr)
	{
		// Render Finish
		RSI->EndFrame();
	}

	// Check Timer and Increase Frame Stat
	FrameCount++;
	FrameMilliSec = double(WindowsTimer::CheckMilliSeconds() - StartFrameSec) / MilliSecFrequency;
	FrameSec = (float)(FrameMilliSec / 1000);
	ElapsedTime += FrameSec;

	FrameFPS = FrameSec == 0 ? 0 : 1 / FrameSec;
}


