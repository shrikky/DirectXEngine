#include "GameObject.h"



GameObject::GameObject(Mesh *mesh, Material* material)
{
	InitializeRigidBody();

	gameObjectMesh = mesh;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	gameObjectmaterial = material;

	//InitializeRigidBody();

	SetWorldMatrix();
}


GameObject::~GameObject()
{
	
}

void GameObject::Draw(ID3D11DeviceContext* deviceContext) {

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Draw the mesh
	ID3D11Buffer* temp = gameObjectMesh->GetVertexBuffer(); // Returns vBuffer which is a pointer to the ID3D11, that it 
	deviceContext->IASetVertexBuffers(0, 1, &temp, &stride, &offset);
	deviceContext->IASetIndexBuffer(gameObjectMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER

	deviceContext->DrawIndexed(
		gameObjectMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,								// Offset to the first index we want to use
		0);
}
void GameObject::SetWorldMatrix() {
	XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rot = XMMatrixRotationY(rotation.y);
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX model = scaleMatrix * rot * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(model));	//Setting matrix as transpose
	
}
void GameObject::Move() {
	position.x = 5.0f;
	SetWorldMatrix();
}

void GameObject::Strafe(float disp) {

	XMVECTOR objPos = XMLoadFloat3(&position);
	XMVECTOR _right = -XMLoadFloat3(&leftVector);
	objPos += _right * disp;
	XMStoreFloat3(&leftVector, _right);
	XMStoreFloat3(&position, objPos);

	SetWorldMatrix();

}

void GameObject::PrepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj) {

	gameObjectmaterial->vertexShader->SetMatrix4x4("world", worldMatrix);
	gameObjectmaterial->vertexShader->SetMatrix4x4("model", modelMatrix);
	gameObjectmaterial->vertexShader->SetMatrix4x4("view", view);
	gameObjectmaterial->vertexShader->SetMatrix4x4("projection", proj);
	gameObjectmaterial->UpdateShaderResources();
	gameObjectmaterial->vertexShader->SetShader(true);
	gameObjectmaterial->pixelShader->SetShader(true);
}

void GameObject::MoveForward() {

}

void GameObject::InitializeRigidBody() {
	//create a dynamic rigidbody

	colShape = new btSphereShape(btScalar(1));
	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	startTransform.setIdentity();

	mass = 10.0f;

	localInertia = btVector3(0, 0, 0);
	colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	body = new btRigidBody(rbInfo);



	//mPlayerBox = new btBoxShape(btVector3(1, 1, 1));
	//mPlayerObject = new btCollisionObject();
	//mPlayerObject->setCollisionShape(mPlayerBox);

	//mPlayerObject->setWorldTransform(startTransform);
	//mPlayerObject->forceActivationState(DISABLE_DEACTIVATION);//maybe not needed
	

}