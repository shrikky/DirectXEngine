
// Constant Buffer
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};
// Vertex data
struct VertexShaderInput
{ 
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

// Output struct of Vertex shader
struct VertexToPixel
{	

	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;   
	float2 uv			: TEXCOORD;
     // RGBA color
};

VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.normal = mul(input.normal, (float3x3)world );
	output.normal = normalize(input.normal);
	output.tangent = mul(input.tangent, (float3x3)world);
	output.worldPos = mul(float4(input.position, 1.0f),world).xyz;
	output.uv = input.uv;

	return output;
}