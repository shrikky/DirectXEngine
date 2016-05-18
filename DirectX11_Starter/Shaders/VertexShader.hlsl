
// Constant Buffer
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix shadowView;
	matrix shadowProjection;
};
// Vertex data
struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

// Output struct of Vertex shader
struct VertexToPixel
{	

	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos     : TEXCOORD0;
	float2 uv           : TEXCOORD1;
	float4 posForShadow : TEXCOORD2;
	// RGBA color
};

VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.normal = mul(input.normal, (float3x3)world );
	output.tangent = mul(input.tangent, (float3x3)world);

	output.normal = normalize(input.normal);
	output.worldPos = mul(float4(input.position, 1.0f),world).xyz;
	output.uv = input.uv;

	// Calculate output position in relation to the light source
	matrix shadowWVP = mul(mul(world, shadowView), shadowProjection);
	output.posForShadow = mul(float4(input.position, 1), shadowWVP);
	return output;
}