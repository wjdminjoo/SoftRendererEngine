
#include "Precompiled.h"
#include "SoftRendererImpl2D.h"
#include "SoftRenderer.h"

SoftRendererImpl2D::SoftRendererImpl2D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();
}

SoftRendererImpl2D::~SoftRendererImpl2D()
{
}

void SoftRendererImpl2D::DrawGrid2D()
{
	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// Render Start
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenHalfSize = ScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= ScreenSize.X; x += Grid2DUnit)
	{
		RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= ScreenSize.Y; y += Grid2DUnit)
	{
		RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}


void SoftRendererImpl2D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	DrawGrid2D();

	float sa, ca;
	Math::GetSinCos(sa, ca, Angle);

	Matrix4x4 rotMat(Vector3(ca, sa, 0.f), Vector3(-sa, ca, 0.f), Vector4::UnitZ, Vector4::UnitW);

	const int vertexCount = 4;
	VertexData v[vertexCount] = {
	 VertexData(Vector4(0.5f, 0.5f, 0.f) * 10.f, LinearColor::Red),
	 VertexData(Vector4(2.5f, -2.5f, 0.f) * 10.f, LinearColor::Blue),
	 VertexData(Vector4(-2.5f, -2.5f, 0.f) * 10.f, LinearColor::Green),
	 VertexData(Vector4(0.6f, -1.2f, 0.f) * 10.f, LinearColor::Black)
	}; /*VertexData v[vertexCount] = {
	 VertexData(rotMat * Vector4(-0.5f, -0.7f, 0.f) * 100.f, LinearColor::Red),
	 VertexData(rotMat * Vector4(-0.7f, 0.9f, 0.f) * 100.f, LinearColor::Blue),
	 VertexData(rotMat * Vector4(0.5f, 0.6f, 0.f) * 100.f, LinearColor::Green),
	 VertexData(rotMat * Vector4(0.6f, -1.2f, 0.f) * 100.f, LinearColor::Black)
	};*/

	const int triangleCount = 2;
	const int indexCount = triangleCount * 3;
	int i[indexCount] = {
	 0, 2, 1, 0, 3, 2
	};

	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawPrimitive(3, 3);
}

void SoftRendererImpl2D::UpdateImpl(float DeltaSeconds)
{
	Angle += (180.f * DeltaSeconds);
}