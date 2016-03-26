#include "Material.h"


Material::Material(SimpleVertexShader* vert, SimplePixelShader* pix) {
	vertexShader = vert;
	pixelShader = pix;
}

Material::Material(SimpleVertexShader* vert, SimplePixelShader* pix, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName) {
	vertexShader = vert;
	pixelShader = pix;

	CreateWICTextureFromFile(device, deviceContext, fileName, 0, &texSRV);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

}

Material::Material(SimpleVertexShader* vert, SimplePixelShader* pix, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* texture, const wchar_t* normalMap) {
	vertexShader = vert;
	pixelShader = pix;
	CreateWICTextureFromFile(device, deviceContext, texture, 0, &texSRV);
	CreateWICTextureFromFile(device, deviceContext, normalMap, 0, &nMap);
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

}
Material::~Material()
{
}
