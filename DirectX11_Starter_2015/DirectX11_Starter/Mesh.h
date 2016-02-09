#pragma once
#include <d3d11.h>
#include "DirectXGameCore.h"
#include "Vertex.h"
class Mesh
{
public:

	// Buffer var
	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;

	//General public Var
	int numOfIndices;

	// Functions
	// Get Vertex Buffer
	ID3D11Buffer* GetVertexBuffer();
	// Get Index Buffer
	ID3D11Buffer* GetIndexBuffer();
	// Get number of Indices
	int GetIndexCount();


	Mesh(int vertNum, Vertex* vert, int indNum, unsigned int* indices, ID3D11Device* deviceObj);

	~Mesh();

};

