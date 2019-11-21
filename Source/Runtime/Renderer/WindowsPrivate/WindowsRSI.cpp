
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "WindowsGDI.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	SetBlendingMode(BlendingMode);
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::SetBlendingMode(BlendingModes InNewBlendingMode)
{
	BlendingMode = InNewBlendingMode;
	//using namespace std::placeholders;
	//if (BlendingMode == BlendingModes::Opaque)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelOpaque, this, _1, _2);
	//}
	//else if (BlendingMode == BlendingModes::AlphaBlending)
	//{
	//	SetPixel = std::bind(&WindowsGDI::SetPixelAlphaBlending, this, _1, _2);
	//}
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(ScreenSize, InVectorPos), InColor);
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < 0 || InX >= ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}

	return;
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY < 0 || InY >= ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::SetPixel(const ScreenPoint& InPos, const LinearColor& InColor)
{
	if (BlendingMode == BlendingModes::Opaque)
	{
		SetPixelOpaque(InPos, InColor);
	}
	else if (BlendingMode == BlendingModes::AlphaBlending)
	{
		SetPixelAlphaBlending(InPos, InColor);
	}
}


void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	ScreenPoint startPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InStartPos);
	ScreenPoint endPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InEndPos);

	int w = endPos.X - startPos.X;
	int h = endPos.Y - startPos.Y;

	bool slope = (Math::Abs(w) >= Math::Abs(h));
	int dirx = w > 0 ? 1 : -1;
	int diry = h > 0 ? 1 : -1;
	int fw = dirx * w;
	int fh = diry * h;

	int f = slope ? fh * 2 - fw : 2 * fw - fh;
	int f1 = slope ? 2 * fh : 2 * fw;
	int f2 = slope ? 2 * (fh - fw) : 2 * (fw - fh);
	int x = startPos.X;
	int y = startPos.Y;

	if (slope)
	{
		while (x != endPos.X)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += diry;
			}

			x += dirx;
		}
	}
	else
	{
		while (y != endPos.Y)
		{
			SetPixel(ScreenPoint(x, y), InColor);

			if (f < 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dirx;
			}

			y += diry;
		}
	}
	//ScreenPoint startPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InStartPos);
	//ScreenPoint endPos = ScreenPoint::ToScreenCoordinate(ScreenSize, InEndPos);


	//int w = endPos.X - startPos.X;
	//int h = InEndPos.Y - startPos.Y;

	//int fh = h * 2 - w;
	//int f1 = 2 * h;
	//int f2 = (h - w) * 2;

	//int fw = w * 2 - h;
	//int f3 = 2 * w;
	//int f4 = (w - h) * 2;

	//int f5 = 2 * (-w - h);
	//int f6 = 2 * (-h - w);
	//int y = InStartPos.Y;
	//int x = InStartPos.X;

	//if (x == 0 && y == 0)
	//{
	//	return;
	//}


	//SetPixel(ScreenPoint(x, y), InColor);
	//while (abs(x) <= abs(InEndPos.X) || abs(y) <= abs(InEndPos.Y))
	//{
	//	if (w > 0 && h > 0) // x y
	//	{
	//		if (w > h)
	//		{
	//			if (fh < 0)
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.0f, 1.0f, 0.4f).ToColor32());
	//				fh += f1;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(1.0f, 0.5f, 0.3f).ToColor32());
	//				fh += f2;
	//				y++;
	//			}
	//			x++;
	//		}
	//		else
	//		{
	//			if (fw < 0)
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.0f, 1.0f, 0.0f).ToColor32());
	//				fw += f3;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(1.0f, 1.0f, 1.0f).ToColor32());
	//				fw += f4;
	//				x++;
	//			}
	//			y++;
	//		}
	//	}
	//	else if (w < 0 && h > 0) // -x y)
	//	{
	//		if (-w < h)
	//		{
	//			if (-fw > 0)
	//			{

	//				SetPixel(ScreenPoint(x, y), LinearColor(0.0f, .6f, 0.2f).ToColor32());
	//				fw -= f3;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(1.0f, 0.3f, 0.5f).ToColor32());
	//				fw += f5;
	//				x--;
	//			}
	//			y++;
	//		}
	//		else
	//		{

	//			if (-fh < 0)
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.8f, .7f, .7f).ToColor32());
	//				fh -= f1;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.2f, 0.6f, 0.7f).ToColor32());
	//				fh += f6;
	//				y++;
	//			}
	//			x--;
	//		}
	//	}
	//	else if (w < 0 && h < 0) // -x -y)
	//	{
	//		if (-w > -h)
	//		{
	//			if (fh > 0)
	//			{

	//				SetPixel(ScreenPoint(x, y), LinearColor(0.0f, .3f, 0.2f).ToColor32());
	//				fh += f1;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(.4f, 0.3f, 0.5f).ToColor32());
	//				fh += f2;
	//				y--;
	//			}
	//			x--;
	//		}
	//		else
	//		{

	//			if (fw > 0)
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.8f, .7f, .3f).ToColor32());
	//				fw += f3;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.0f, 0.2f, 0.7f).ToColor32());
	//				fw += f4;
	//				x--;
	//			}
	//			y--;
	//		}
	//	}
	//	else if (w > 0 && h < 0) // x -y
	//	{
	//		if (w > -h)
	//		{
	//			if (-fh > 0)
	//			{

	//				SetPixel(ScreenPoint(x, y), LinearColor(1.0f, .5f, 0.2f).ToColor32());
	//				fh -= f1;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(1.0f, 0.5f, 0.1f).ToColor32());
	//				fh += f6;
	//				y--;
	//			}
	//			x++;
	//		}
	//		else
	//		{

	//			if (-fw < 0)
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.8f, .7f, .7f).ToColor32());
	//				fw -= f3;
	//			}
	//			else
	//			{
	//				SetPixel(ScreenPoint(x, y), LinearColor(0.f, 0.6f, 0.7f).ToColor32());
	//				fw += f5;
	//				x++;
	//			}
	//			y--;
	//		}
	//	}

	//}
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{

	const int triangleCount = InIndexSize / 3;

	for (int i = 0; i < triangleCount; i++) {
		VertexData tp[3] = { VertexBuffer[IndexBuffer[i * 3]], VertexBuffer[IndexBuffer[i * 3 + 1]], VertexBuffer[IndexBuffer[i * 3 + 2]] };

		ScreenPoint tPoint[4] = {
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[0].Position.ToVector2()),
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[1].Position.ToVector2()),
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[2].Position.ToVector2()),
			ScreenPoint(0, 0)
		};
		// 1. y값으로 점을 sorting
		// 2. 삼각형이 Top-Flat, Bottom-Flat, 무작위 인지 파악
		// 3. Top-Flat은 Top-Flat방식으로 아래부터 위로 그리기
		// 4. Bottom-Flat은 Bottom-Flat 방식으로 위에서 아래로 그리기
		// 5. 무작위는 점을 하나 추가하고 Bottom-Flat과 Top-Flat으로 나눠그리기
		if (tp[0].Position.Y >= tp[1].Position.Y) {
			if (tp[0].Position.Y >= tp[2].Position.Y) {
				// 0, 1, 2
				if (tp[1].Position.Y >= tp[2].Position.Y) {
					// 0, 1, 2
				}
				else {
					// 0, 2, 1
					VertexData temp = tp[1];
					tp[1] = tp[2];
					tp[2] = temp;
				}
			}
			else {
				// 2, 0, 1
				VertexData temp = tp[0];
				tp[0] = tp[2];
				tp[2] = tp[1];
				tp[1] = temp;
			}
		}
		else {
			if (tp[0].Position.Y >= tp[2].Position.Y) {
				// 1, 2, 0
				VertexData temp = tp[0];
				tp[0] = tp[1];
				tp[1] = tp[2];
				tp[2] = temp;
				if (tp[1].Position.Y >= tp[2].Position.Y) {
					// 1, 2, 0
				}
				else {
					// 2, 1, 0
					VertexData temp = tp[0];
					tp[0] = tp[1];
					tp[1] = temp;
				}
			}
			else {
				// 1, 0, 2
				VertexData temp = tp[1];
				tp[1] = tp[2];
				tp[2] = temp;
			}
		}

		if (tp[0].Position.Y == tp[1].Position.Y) {
			// Top-Flat Triangle

			//tp[2] tp[0] 기울기
			float slopeLeft = (tp[0].Position.X - tp[2].Position.X) / (tp[0].Position.Y - tp[2].Position.Y);
			//tp[2] tp[1] 기울기
			float slopeRight = (tp[1].Position.X - tp[2].Position.X) / (tp[1].Position.Y - tp[2].Position.Y);

			DrawLine(tp[0].Position.ToVector2(), tp[1].Position.ToVector2(), LinearColor::Blue);
			DrawLine(tp[1].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Blue);
			DrawLine(tp[0].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Blue);

			float y = tp[2].Position.Y;

			while (y < tp[0].Position.Y) {
				float leftX = (y - tp[2].Position.Y) * slopeLeft + tp[2].Position.X;
				float rightX = (y - tp[2].Position.Y) * slopeRight + tp[2].Position.X;

				ScreenPoint spLeft = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(leftX, y));
				ScreenPoint spRight = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(rightX, y));

				for (int x = spLeft.X; x <= spRight.X; x++) {
					SetPixel(ScreenPoint(x, spLeft.Y), LinearColor::Blue);
				}
				y += 1.f;
			}

		}
		else {
			if (tp[1].Position.Y == tp[2].Position.Y) {
				// Bottom-Flat Triangle

				float slopeLeft = (tp[2].Position.X - tp[0].Position.X) / (tp[2].Position.Y - tp[0].Position.Y);
				float slopeRight = (tp[1].Position.X - tp[0].Position.X) / (tp[1].Position.Y - tp[0].Position.Y);

				DrawLine(tp[0].Position.ToVector2(), tp[1].Position.ToVector2(), LinearColor::Red);
				DrawLine(tp[1].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Red);
				DrawLine(tp[0].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Red);

				float y = tp[0].Position.Y;

				while (y > tp[2].Position.Y) {
					float leftX = (tp[0].Position.Y - y) * slopeLeft + tp[2].Position.X;
					float rightX = (tp[0].Position.Y - y) * slopeRight + tp[1].Position.X;

					ScreenPoint spLeft = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(leftX, y));
					ScreenPoint spRight = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(rightX, y));

					if (spRight.X < spLeft.X) {
						ScreenPoint temp = spLeft;
						spLeft = spRight;
						spRight = temp;
					}

					for (int x = spLeft.X; x <= spRight.X; x++) {
						SetPixel(ScreenPoint(x, spLeft.Y), LinearColor::Red);
					}
					y -= 1.f;
				}
			}
			else {
				// half Triangle

			// Top-Flat Triangle
				float slopeLeft = (tp[0].Position.X - tp[2].Position.X) / (tp[0].Position.Y - tp[2].Position.Y);
				float slopeRight = (tp[1].Position.X - tp[2].Position.X) / (tp[1].Position.Y - tp[2].Position.Y);

				DrawLine(tp[0].Position.ToVector2(), tp[1].Position.ToVector2(), LinearColor::Green);
				DrawLine(tp[1].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Green);
				DrawLine(tp[0].Position.ToVector2(), tp[2].Position.ToVector2(), LinearColor::Green);

				float y = tp[2].Position.Y;

				while (y < tp[0].Position.Y) {
					float leftX = (y - tp[2].Position.Y) * slopeLeft + tp[2].Position.X;
					float rightX = (y - tp[2].Position.Y) * slopeRight + tp[2].Position.X;

					ScreenPoint spLeft = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(leftX, y));
					ScreenPoint spRight = ScreenPoint::ToScreenCoordinate(ScreenSize, Vector2(rightX, y));

					for (int x = spLeft.X; x <= spRight.X; x++) {
						SetPixel(ScreenPoint(x, spLeft.Y), LinearColor::Green);
					}
					y += 1.f;
				}

			}
		}
	}
}