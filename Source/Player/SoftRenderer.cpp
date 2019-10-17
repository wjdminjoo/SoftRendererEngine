
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
///<summary>
/// y = a * x + b
/// y = w / h * x + b
///</summary>

void SoftRenderer::Update()
{

	if (RSI == nullptr) return;

	float sinWave = (sinf(ElapsedTime * Math::TwoPI) + 1 ) * 0.5f;
	float cosWave = (cos(ElapsedTime * Math::TwoPI) + 1) * 0.5f;
	float tanWave = (tan(ElapsedTime * Math::TwoPI) + 1) * 0.5f;


	// Colors to use
	LinearColor vectorSpaceColor(LinearColor(0.125f, 0.5f, 1.0f, 0.3f));

	// Basis Vector forms Vector Space
	Vector3 xBasisVector(1.f, .0f, 0.f);
	Vector3 yBasisVector(.0f, 1.f, 0.f);
	Vector3 zBasisVector(.0f, .0f, 1.f);
	// Scalar to use
	float scalar = 100.f;
	float theta = Math::Deg2Rad(50.f);
	float sinTheta, cosTheta;
	Math::SinCos(&sinTheta, &cosTheta, theta);
	//TEST
	float moveSpeed = 100.0f;

	static Vector2 currentPos = Vector2::Zero;
	Vector2 deltaPos = Vector2(inputManager.GetXAxis(), inputManager.GetYAxis()) * moveSpeed * FrameSec;
	currentPos += deltaPos;

	//TRS
	// To Do : Define Transform Matrix 3x3
	static GameObject2D quad;
	quad.GetTransform().AddPosition(deltaPos);
	//quad.GetTransform().AddScale(Vector2(1.f, 1.f));
	//quad.GetTransform().AddRotation(FrameSec);
	Matrix3x3 m = quad.Transform.GetTRS();

	/*VertexData v[4];
	v[0].Position = (m * Vector3(Vector2::Zero, true)).ToVector2();
	v[0].Color = vectorSpaceColor;

	v[1].Position = (m * Vector3(Vector2::UnitX, true)).ToVector2();
	v[1].Color = vectorSpaceColor;

	v[2].Position = (m * Vector3(Vector2::UnitY, true)).ToVector2();
	v[2].Color = vectorSpaceColor;

	v[3].Position = (m * Vector3(Vector2::UnitX + Vector2::UnitY, true)).ToVector2();
	v[3].Color = vectorSpaceColor;*/

	int i[6];
	i[0] = 0;
	i[1] = 3;
	i[2] = 1;
	i[3] = 0;
	i[4] = 2;
	i[5] = 3;

	// Draw Call
	/*RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawPrimitive(4, 6);*/

	// Draw Basis Vector Arrow
	/*RSI->DrawArrowLine(v[0].Position, v[1].Position, LinearColor::Red, 4);
	RSI->DrawArrowLine(v[0].Position, v[2].Position, LinearColor::Green, 4);*/

	/*RSI->DrawLine2(ScreenPoint(10, 10), ScreenPoint(120, 50));
	RSI->DrawLine2(ScreenPoint(10, 10), ScreenPoint(50, 120));
	RSI->DrawLine2(ScreenPoint(-10, 10), ScreenPoint(-120, 50));
	RSI->DrawLine2(ScreenPoint(-10, 10), ScreenPoint(-50, 120));
	
	RSI->DrawLine2(ScreenPoint(-10, -10), ScreenPoint(-120, -50));
	RSI->DrawLine2(ScreenPoint(-10, -10), ScreenPoint(-50, -120));
	RSI->DrawLine2(ScreenPoint(10, -10), ScreenPoint(120, -50));
	RSI->DrawLine2(ScreenPoint(10, -10), ScreenPoint(50, -120));*/
	
	VertexData v[4];
	v[0].Position = Vector2(-50.f, -50.f);
	v[1].Position = Vector2(-50.f, 50.f);
	v[2].Position = Vector2(50.f, 50.f);
	v[3].Position = Vector2(50.f, -50.f);

	static int indexCount = 6;
	int index[6] = { 0, 1, 2, 0, 2, 3 };
	int triangleCount = indexCount / 3;

	for (int i = 0; i < triangleCount; i++) {
		RSI->DrawLine2(ScreenPoint(v[index[i * 3]].Position), ScreenPoint(v[index[i * 3 + 1]].Position));
		//RSI->DrawLine2(ScreenPoint(v[index[i * 3]].Position), ScreenPoint(v[index[i * 3 + 2]].Position));
		//RSI->DrawLine2(ScreenPoint(v[index[i * 3 + 1]].Position), ScreenPoint(v[index[i * 3 + 2]].Position));
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

	// Draw Line2

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


