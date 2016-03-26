#pragma once
#include "SimpleShader.h"
#include "WICTextureLoader.h"
#include "DirectXGameCore.h"

using namespace DirectX;
class Material
{
public:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* texSRV;
	ID3D11ShaderResourceView* nMap;
	ID3D11SamplerState* samplerState;
	Material(SimpleVertexShader* vert, SimplePixelShader* pix);
	Material(SimpleVertexShader* vert, SimplePixelShader* pix,ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* textName);
	Material::Material(SimpleVertexShader* vert, SimplePixelShader* pix, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* texture, const wchar_t* normalMap);
	~Material();
};

