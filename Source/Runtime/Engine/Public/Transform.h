#pragma once

class Transform
{
public:
	Transform() = default;

public:
	void SetPostion(Vector3 InPostion) { Position = InPostion; }
	void SetRotaion(Vector3 InRotation) { Rotation = InRotation; }
	void SetScale(Vector3 InScale) { Scale = InScale; }

	void AddPostion(Vector3 InPostion) { Position += InPostion; }
	void AddRotation(Vector3 InRotation) { Rotation += InRotation; }
	void AddScale(Vector3 InScale) { Scale += InScale; }

	Vector3 GetPosition() const { return Position; }
	Vector3 GetRotation() const  { return Rotation; }
	Vector3 GetScale() const  { return Scale; }
	Matrix4x4 GetModelingMatrix() const;
private:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
};

