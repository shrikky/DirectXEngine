#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include <d3d11.h>
#include "Material.h"
#include "imgui_impl_dx11.h"
#include "btBulletDynamicsCommon.h"

#include"Physics.h"

using namespace DirectX;
class GameObject
{
public:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 modelMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	Mesh* gameObjectMesh;
	Material* gameObjectmaterial;
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

	void SetRotationX(float x) {
		rotation.x = x;
		SetWorldMatrix();
	}

	void SetRotationY(float y) {
		rotation.y = y;
		SetWorldMatrix();
	}


	void SetRotationZ(float z) {
		rotation.z = z;
		SetWorldMatrix();
	}

	void SetScale(XMFLOAT3 scale) {
		 position = scale;
	}

	void Move();
	void SetWorldMatrix();
	void MoveForward();
	void Draw(ID3D11DeviceContext* deviceContext);
	void PrepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj);
	GameObject(Mesh* mesh, Material* material);
	~GameObject();

	//create a dynamic rigidbody
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	btCollisionShape* colShape;

	/// Create Dynamic Objects
	btTransform startTransform;

	float mass;

	btVector3 localInertia;

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState;
	btRigidBody* body;

	void InitializeRigidBody();


private:


};

