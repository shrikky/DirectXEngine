#include "GameObject.h"



GameObject::GameObject(Mesh *mesh, Material* material)
{
	DistanceBased = false;
	gameObjectMesh = mesh;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	gameObjectmaterial = material;
	SetWorldMatrix();
}

GameObject::GameObject(Mesh * mesh, Material * materialClose, Material * materialMed, Material * materialFar, Camera* cam)
{
	DistanceBased = true;
	mainCam = cam;
	gameObjectMesh = mesh;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	gameObjectmaterialClose = materialClose;
	gameObjectmaterialMed = materialMed;
	gameObjectmaterialFar= materialFar;
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
	//XMMATRIX rot = XMMatrixRotationY(rotation.y);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX model = scaleMatrix * rot * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(model));	//Setting matrix as transpose
}
float GameObject::distance(XMFLOAT3 pt1, XMFLOAT3 pt2)
{
	//XMFLOAT3 v = pt2 - pt1;
	//return sqrt(dot(v, v));
	XMVECTOR temp1 = XMVectorSet(pt1.x, pt1.y, pt1.z, 0);
	XMVECTOR temp2 = XMVectorSet(pt2.x, pt2.y, pt2.z, 0);
	XMVECTOR value = XMVector3Dot(temp1, temp2);
	float temp = (pow((pt2.x - pt1.x), 2) + pow((pt2.y - pt1.y), 2) + pow((pt2.z - pt1.z), 2));
	if (Framecount % 2000 == 0) {
		temp = temp;

	}
	return sqrt(temp);
	//return 1.0f;
}
void GameObject::Move() {
	position.x = 5.0f;
	SetWorldMatrix();
}
void GameObject::PrepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj) {
	if (!DistanceBased) {
		gameObjectmaterial->vertexShader->SetMatrix4x4("world", worldMatrix);
		gameObjectmaterial->vertexShader->SetMatrix4x4("model", modelMatrix);
		gameObjectmaterial->vertexShader->SetMatrix4x4("view", view);
		gameObjectmaterial->vertexShader->SetMatrix4x4("projection", proj);
		gameObjectmaterial->UpdateShaderResources();
		gameObjectmaterial->vertexShader->SetShader(true);
		gameObjectmaterial->pixelShader->SetShader(true);
	}else{
		Framecount += 1;
		float TempDistToCamera = GameObject::distance(position, mainCam->GetCamePos());
		//GetDistToPlayerCamera (mainCam)
		if (TempDistToCamera <= 20) {
			//rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
			gameObjectmaterialClose->vertexShader->SetMatrix4x4("world", worldMatrix);
			gameObjectmaterialClose->vertexShader->SetMatrix4x4("model", modelMatrix);
			gameObjectmaterialClose->vertexShader->SetMatrix4x4("view", view);
			gameObjectmaterialClose->vertexShader->SetMatrix4x4("projection", proj);
			gameObjectmaterialClose->UpdateShaderResources();
			gameObjectmaterialClose->vertexShader->SetShader(true);
			gameObjectmaterialClose->pixelShader->SetShader(true);
		}
		else if (TempDistToCamera <= 40) {
			//rotation = XMFLOAT3(0.5f, 0.0f, 0.0f);
			gameObjectmaterialMed->vertexShader->SetMatrix4x4("world", worldMatrix);
			gameObjectmaterialMed->vertexShader->SetMatrix4x4("model", modelMatrix);
			gameObjectmaterialMed->vertexShader->SetMatrix4x4("view", view);
			gameObjectmaterialMed->vertexShader->SetMatrix4x4("projection", proj);
			gameObjectmaterialMed->UpdateShaderResources();
			gameObjectmaterialMed->vertexShader->SetShader(true);
			gameObjectmaterialMed->pixelShader->SetShader(true);
		}
		else {
			//rotation = XMFLOAT3(0.0f, 0.0f, 3.7f);
			gameObjectmaterialFar->vertexShader->SetMatrix4x4("world", worldMatrix);
			gameObjectmaterialFar->vertexShader->SetMatrix4x4("model", modelMatrix);
			gameObjectmaterialFar->vertexShader->SetMatrix4x4("view", view);
			gameObjectmaterialFar->vertexShader->SetMatrix4x4("projection", proj);
			gameObjectmaterialFar->UpdateShaderResources();
			gameObjectmaterialFar->vertexShader->SetShader(true);
			gameObjectmaterialFar->pixelShader->SetShader(true);

		}
	
	}
}
//float distance(XMFLOAT3 pt1, XMFLOAT3 pt2)
//{
//	//XMFLOAT3 v = pt2 - pt1;
//	//return sqrt(dot(v, v));
//	XMVECTOR temp1 = XMVectorSet(pt1.x,pt1.y,pt1.z, 0);
//	XMVECTOR temp2 = XMVectorSet(pt2.x, pt2.y, pt2.z, 0);
//	XMVECTOR value = XMVector3Dot(temp1, temp2);
//	float temp = (pow((pt2.x - pt1.x),2) + pow((pt2.y - pt1.y),2) + pow((pt2.z - pt1.z),2));
//	
//	return sqrt(temp);
//
//	//return 1.0f;
//}



void GameObject::MoveForward() {

}