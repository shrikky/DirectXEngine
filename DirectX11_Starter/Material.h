#pragma once
#include "SimpleShader.h"
class Material
{
public:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Material(SimpleVertexShader* vert, SimplePixelShader* pix);
	~Material();
};

