#include "Mesh.h"



Mesh::Mesh(int vertNum, Vertex* vert, int indNum, unsigned int* indices, ID3D11Device* deviceObj)
{
	numOfIndices = indNum;
	// Create vertexBuffers
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertNum;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	// Load Data to Vertex Buffer
	D3D11_SUBRESOURCE_DATA vertData;
	vertData.pSysMem = vert;

	// Create the buffer with the data
	HR(deviceObj->CreateBuffer(&vbd, &vertData, &vBuffer));

	//Create Index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indNum;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	//Load Data to IndexBuffer
	D3D11_SUBRESOURCE_DATA indData;
	indData.pSysMem = indices;

	// Create Index Buffer with the data.
	HR(deviceObj->CreateBuffer(&ibd, &indData, &iBuffer));
}


Mesh::~Mesh()
{
	// Release any D3D stuff that's still hanging out
	ReleaseMacro(vBuffer);
	ReleaseMacro(iBuffer);

}
ID3D11Buffer* Mesh::GetIndexBuffer() {
	return iBuffer;
}
int Mesh::GetIndexCount() {
	return numOfIndices;
}
ID3D11Buffer* Mesh::GetVertexBuffer() {
	return vBuffer;
}