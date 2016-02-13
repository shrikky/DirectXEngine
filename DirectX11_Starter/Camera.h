#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
using namespace DirectX;
class Camera
{
public:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT3 camPosition = XMFLOAT3(0, 0, -5);
	XMFLOAT3 forwardVector = XMFLOAT3(0, 0, 1);
	XMFLOAT3 upDirection = XMFLOAT3(0, 1, 0);
	XMFLOAT3 leftVector = XMFLOAT3(0, 0, 0);
	float rotY = 0.0f;
	float rotX = 0.0f;
	float rotZ = 0.0f;
	XMFLOAT4X4 GetWorldMatrix(){
		return worldMatrix;
	}
	XMFLOAT4X4 GetviewMatrix() {
		return viewMatrix;
	}
	void SetRotationX(float x) {
		rotX = x;
	}
	void SetRotationY(float y) {
		rotY = y;
	}
	void SetRotationZ(float z) {
		rotZ = z;
	}
	void Update();
	void Forward(float disp);
	void Rotate(float rotation);
	void Strafe(float disp); 
	void VerticalMovement(float disp);
	Camera();
	~Camera();
};

