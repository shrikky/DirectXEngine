#include "Camera.h"
#include <iostream>


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::Update() {

		XMVECTOR _forward = XMLoadFloat3(&forwardVector);
		XMVECTOR _camPos = XMLoadFloat3(&camPosition);
		XMVECTOR _upAxis = XMLoadFloat3(&upDirection);
		XMMATRIX _view = XMLoadFloat4x4(&viewMatrix);
		XMVECTOR _left = XMVector3Cross(_forward, _upAxis);
		XMStoreFloat3(&leftVector, _left);
  		_view = XMMatrixLookToLH(_camPos, _forward, _upAxis);
		XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(_view));
}

void Camera::Forward(float disp) {
	XMVECTOR camPos = XMLoadFloat3(&camPosition);
	XMVECTOR _forward = XMLoadFloat3(&forwardVector);
	camPos += _forward * disp;
	XMStoreFloat3(&camPosition,camPos);

}

void Camera::Rotate(float rot) {

	XMVECTOR forwardDir = XMLoadFloat3(&forwardVector);
	XMVECTOR camPos = XMLoadFloat3(&camPosition);
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(0, rot,0);  // Rotate y by 30
	forwardDir = XMVector3Rotate(forwardDir, rotation);
	XMStoreFloat3(&forwardVector, forwardDir);

}

void Camera::VerticalMovement(float disp) {

	XMVECTOR camPos = XMLoadFloat3(&camPosition);
	XMVECTOR _upVector = XMLoadFloat3(&upDirection);
	camPos += _upVector * disp;
	XMStoreFloat3(&camPosition, camPos);
}
void Camera::Strafe(float disp) {

	XMVECTOR camPos = XMLoadFloat3(&camPosition);
	XMVECTOR _right = -XMLoadFloat3(&leftVector);
	camPos += _right * disp;
	XMStoreFloat3(&leftVector, _right);
	XMStoreFloat3(&camPosition, camPos);

}