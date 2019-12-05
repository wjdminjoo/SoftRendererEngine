
#include "Precompiled.h"
#include "SoftRendererImpl3D.h"
#include "SoftRenderer.h"

SoftRendererImpl3D::SoftRendererImpl3D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();

	LoadResource();
	LoadScene();
}

SoftRendererImpl3D::~SoftRendererImpl3D()
{
}


void SoftRendererImpl3D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	auto player = Scene[0].get(); // 해시값이나 맵을 이용한 값을 넣자.

	for (const auto& g : Scene) {
		Mesh* meshToRender = g->Getmesh();
		int vertexCount = meshToRender->GetVertexCount();
		int indexCount = meshToRender->GetIndexCount();

		Vector4* meshVertexBuffer = meshToRender->GetVertices();
		int* indexBuffuer = meshToRender->GetIndices();

		VertexData* v = new VertexData[vertexCount];

		for (int vi = 0; vi < vertexCount; ++vi) {
			v[vi].Position = meshVertexBuffer[vi];
		}

		Matrix4x4 unitforMatrix[3] = {
			g->GetTransform().GetModelingMatrix(),
			MainCamera.GetViewMatrix(*player), // 바라볼 대상
			MainCamera.GetProjectionMatrix(ScreenSize.X, ScreenSize.Y)
		};

		RSI->SetUnitformMatrix(unitforMatrix);
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(indexBuffuer);
		RSI->DrawPrimitive(vertexCount, indexCount);
		delete[] v;
	}


	/*float sa, ca;
	Math::GetSinCos(sa, ca, Angle); 

	Matrix4x4 rotMat(Vector3(ca, sa, 0.f), Vector3(-sa, ca, 0.f), Vector4::UnitZ, Vector4::UnitW);

	const int vertexCount = 4;
	VertexData v[vertexCount] = {
	 VertexData(Vector4(-0.5f, -0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(-0.5f, 0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(0.5f, 0.5f, 0.f) * 100.f, LinearColor::Black),
	 VertexData(Vector4(0.6f, -1.2f, 0.f) * 100.f, LinearColor::Black)
	};

	const int triangleCount = 2;
	const int indexCount = triangleCount * 3;
	int i[indexCount] = {
	 0, 2, 1, 0, 3, 2
	};

	RSI->SetVertexBuffer(v);
	RSI->SetIndexBuffer(i);
	RSI->DrawPrimitive(3, 3);*/
}

void SoftRendererImpl3D::UpdateImpl(float DeltaSeconds)
{
	Angle += (180.f * DeltaSeconds * InputManager.GetXAxis());
	const auto player = Scene[0].get();
	const float moveSpeed = 100.f;
	Vector3 deltaPostion = Vector3(
		InputManager.GetXAxis() * moveSpeed * DeltaSeconds, 
		InputManager.GetYAxis() * moveSpeed * DeltaSeconds, 
		0.f );
	

	MainCamera.GetTransform().AddPostion(deltaPostion);
}

void SoftRendererImpl3D::LoadResource()
{

	// Load Mesh Resource
	const int vertexCount = 24;
	const int triangleCount = 12;
	const int indexCount = triangleCount * 3;

	Vector4* v = new Vector4[vertexCount]{
		// Right 
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		// Front
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		// Back
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f),
		// Left
		Vector4(-0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, -0.5f, 0.5f),
		// Top
		Vector4(0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, 0.5f),
		Vector4(-0.5f, 0.5f, -0.5f),
		Vector4(0.5f, 0.5f, -0.5f),
		// Bottom
		Vector4(-0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, 0.5f),
		Vector4(0.5f, -0.5f, -0.5f),
		Vector4(-0.5f, -0.5f, -0.5f)
	};

	int* i = new int[indexCount] {
		0, 2, 1, 0, 3, 2,
			4, 6, 5, 4, 7, 6,
			8, 10, 9, 8, 11, 10,
			12, 14, 13, 12, 15, 14,
			16, 18, 17, 16, 19, 18,
			20, 22, 21, 20, 23, 22
	};

	// 메시에 넘기기
	SharedMesh = std::make_unique<Mesh>();
	SharedMesh->SetMesh(v, vertexCount, i, indexCount);

	delete[] v;
	delete[] i;

	// 위치
	// 이동
	// 스케일
	// 카메라
	// 프로젝션 -> W값으로 나누기 후 크기만큼 늘리기.
}

void SoftRendererImpl3D::LoadScene()
{
	GameObject* player = new GameObject();
	Scene.emplace_back(player);

	player->SetMesh(SharedMesh.get());

	player->GetTransform().SetPostion(Vector3::Zero);
	player->GetTransform().SetScale(Vector3::One * 100);
	MainCamera.GetTransform().SetPostion(Vector3(0.f, 0.f, -500.0f));
	
}
