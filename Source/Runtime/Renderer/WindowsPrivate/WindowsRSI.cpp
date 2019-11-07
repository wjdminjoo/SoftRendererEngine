
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
	// 0. VertexBuffer�� IndexBuffer�� NULL�� �ƴ��� üũ.
	// 1. IndexSize�κ��� �ﰢ�� �� �ľ�.
	// 2. �ﰢ�� ���� ��ŭ ���鼭 �ﰢ���� Wireframe���� �׸���

	/*if (VertexBuffer != NULL || IndexBuffer != NULL) {
		continue;
	}*/

	const int triangleCount = InIndexSize / 3;

	for (int i = 0; i < triangleCount; i++) {
		VertexData tp[3] = { VertexBuffer[IndexBuffer[i * 3]], VertexBuffer[IndexBuffer[i * 3 + 1]], VertexBuffer[IndexBuffer[i * 3 + 2]] };


		ScreenPoint tPoint[4] = {
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[0].Position.ToVector2()),
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[1].Position.ToVector2()),
			ScreenPoint::ToScreenCoordinate(ScreenSize, tp[2].Position.ToVector2()),
			ScreenPoint(0, 0)
		};
		// 1. y������ ���� sorting
		// 2. �ﰢ���� Top-Flat, Bottom-Flat, ������ ���� �ľ�
		// 3. Top-Flat�� Top-Flat������� �Ʒ����� ���� �׸���
		// 4. Bottom-Flat�� Bottom-Flat ������� ������ �Ʒ��� �׸���
		// 5. �������� ���� �ϳ� �߰��ϰ� Bottom-Flat�� Top-Flat���� �����׸���
		if (tp[0].Position.Y >= tp[1].Position.Y){
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
		else{
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
			//tp[2] tp[0] ����
			float inclination1 = tp[2].Position.Y - tp[0].Position.Y / tp[2].Position.X - tp[0].Position.X;
			//tp[2] tp[1] ����
			float inclination2 = tp[2].Position.Y - tp[1].Position.Y / tp[2].Position.X - tp[1].Position.X;

		}
		else {
			if (tp[1].Position.Y == tp[2].Position.Y) {
				// Bottom-Flat Triangle

			}
			else {
				// half Triangle
				
				
			}
		}

		//WireFrame

		/*DrawLine(tp[0].Position.ToVector2(), tp[1].Position.ToVector2(), tp[0].Color);
		DrawLine(tp[0].Position.ToVector2(), tp[2].Position.ToVector2(), tp[1].Color);
		DrawLine(tp[1].Position.ToVector2(), tp[2].Position.ToVector2(), tp[2].Color);*/
		//}


		//Fill Triangle
		//Find Min, Max ScreenPoint;


		// �ﰢ�� ĥ�ϱ�.
		// ���� : �ﰢ�� 2�� ũ�⸦ ������ ����.
		//Vector2 minPos(INFINITY, INFINITY);
		//Vector2 maxPos(-INFINITY, -INFINITY);

		/*for (int i = 0; i < 3; i++) {
			if (tp[i].Position.X < minPos.X) {
				minPos.X = tp[i].Position.X;
			}
			if (tp[i].Position.Y < minPos.Y) {
				minPos.Y = tp[i].Position.Y;
			}
			if (tp[i].Position.X > maxPos.X) {
				maxPos.X = tp[i].Position.X;
			}
			if (tp[i].Position.Y > maxPos.Y) {
				maxPos.Y = tp[i].Position.Y;
			}
		}
		ScreenPoint minScreenPos = ScreenPoint::ToScreenCoordinate(ScreenSize, minPos);
		ScreenPoint maxScreenPos = ScreenPoint::ToScreenCoordinate(ScreenSize, maxPos);*/

		/*Vector2 u = (tp[1].Position - tp[0].Position).ToVector2();
		Vector2 v = (tp[2].Position - tp[0].Position).ToVector2();

		float Dotuv = u.Dot(v);
		float Dotvv = v.Dot(v);
		float Dotuu = u.Dot(u);

		float invDenominator = 1.f / (Dotuu * Dotvv - Dotuv * Dotuv);
		for (int x = minScreenPos.X; x <= maxScreenPos.X; ++x) {
			for (int y = maxScreenPos.Y; y <= minScreenPos.Y; ++y) {
				ScreenPoint testPoint(x, y);
				Vector2 testPos = testPoint.ToVectorCoordinate(ScreenSize);

				Vector2 w = testPos - tp[0].Position.ToVector2();
				float Dotwu = w.Dot(u);
				float Dotwv = w.Dot(v);

				float s = (Dotwu * Dotvv - Dotwv * Dotuv) * invDenominator;
				float t = (Dotwv * Dotuu - Dotwu * Dotuv) * invDenominator;

				if (s >= 0 && s <= 1.0f && t >= 0.f && t <= 1.f)
					SetPixel(testPoint, LinearColor::Black);


			}
		}*/
	}
}