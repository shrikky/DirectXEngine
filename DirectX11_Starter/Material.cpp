#include "Material.h"


Material::Material(SimpleVertexShader* vert, SimplePixelShader* pix) {
	vertexShader = vert;
	pixelShader = pix;
}


Material::~Material()
{
}
