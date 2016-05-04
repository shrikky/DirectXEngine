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
	XMFLOAT3 leftVector = XMFLOAT3(0, 0, 0);

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

	void SetRotation(float x, float y, float z)
		{
			rotation.x = x;
			rotation.y = y;
			rotation.z = z;
		
			SetWorldMatrix();
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
		SetRigidBodyShape(scale.x/4, scale.y/4,scale.z/4);
		 position = scale;
	}

	void Move();
	void Strafe(float disp);
	void SetWorldMatrix();
	void MoveForward();
	void SetMass(float newmass);
	void SetDefaultMass();
	void CleanupPhysicsObjects();
	void SetRigidBodyShape(float scalex,float scaley,float scalez);
	void SetRigidBodyShape();
	void Draw(ID3D11DeviceContext* deviceContext);
	void PrepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj);
	GameObject(Mesh* mesh, Material* material);
	GameObject(Mesh * mesh, Material * material, float x, float y, float z);
	~GameObject();

	//create a dynamic rigidbody
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	btCollisionShape* colShape;

	/// Create Dynamic Objects
	btTransform  startTransform;

	float mass;

	btVector3 localInertia;

	btDefaultMotionState* myMotionState;
	btRigidBody* body;

	void InitializeRigidBody();


	btBoxShape * mPlayerBox ;
	btCollisionObject * mPlayerObject ;



private:


};

