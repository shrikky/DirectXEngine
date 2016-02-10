#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include <d3d11.h>
using namespace DirectX;
class GameObject
{
public:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	Mesh* gameObjectMesh;

	XMFLOAT4X4 GetWorldMatrix() {
		return worldMatrix;
	}
	XMFLOAT3 GetPosition() {
		return position;
	}
	XMFLOAT3 GetRotation() {
		return position;
	}
	XMFLOAT3 GetScale() {
		return position;
	}

	float GetXPosition() {
		return position.x;
	}
	void SetXPosition(float posx) {
		position.x = posx;
		SetWorldMatrix();
	}
	void SetYPosition(float posy) {
		position.y = posy;
		SetWorldMatrix();
	}
	void SetZPosition(float posz) {
		position.z = posz;
		SetWorldMatrix();
	}

	void SetPosition(XMFLOAT3 pos) {
		position = pos;
	}
	void SetRotation(XMFLOAT3 rot) {
		position = rot;
	}

	void SetRotationY(float y) {
		rotation.y = y;
		SetWorldMatrix();
	}
	void SetScale(XMFLOAT3 scale) {
		 position = scale;
	}

	void Move();
	void SetWorldMatrix();
	void MoveForward();
	void Draw(ID3D11DeviceContext* deviceContext);
	GameObject(Mesh* mesh);
	~GameObject();
private:


};

