
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	
	//float4 color		: COLOR;
};

struct DirectionalLight {
	float4 AmbientColor;		//4
	float4 DiffuseColor;		//4
	float3 Direction;			//3

};
struct PointLight{
	float4 PointLightColor;
	float3 Position;			//3
};
struct SpecularLight{
	float4 SpecularColor;
	float3 Position;			//3
	float SpecularStrength;
};
cbuffer externalData : register(b0)
{
	DirectionalLight directionLight, directionLight2;  //11 x 2
	PointLight pointLight;								//3 
	SpecularLight specularLight; 
	float3 cameraPosition;
};

float4 CalculateDirectionalLight(float3 normal, DirectionalLight light){

	float3 output;
	float3 direction = normalize(-light.Direction);
	normal = normalize(normal);
	float NdotL = saturate(dot(normal,light.Direction));
	output = light.DiffuseColor * NdotL;
	output += light.AmbientColor;
	return float4(output,1);

}
float CalculatePointLight(float3 normal, float3 direction, float dist){
	
	float point_NdotL;
	normal = normalize(normal);
	point_NdotL = saturate(dot(normal, -direction));
	if(dist < 5)
	return point_NdotL;
	else
	return 0;

}

float SpecLight(float normal, float3 direction, float3 cameraDir, SpecularLight light){

	float reflection = reflect(-direction, normal);
	float spec = pow(max(dot(reflection, cameraDir),0),32);
	return spec;

}
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 output;
	float dist = distance (pointLight.Position,input.worldPos);
	float3 pointLDir = normalize(pointLight.Position - input.worldPos);  
	float3 specDir = normalize(specularLight.Position - input.worldPos);
	float3 cameraDir = normalize(cameraPosition - input.worldPos);

	output = CalculateDirectionalLight(input.normal, directionLight) +  specularLight.SpecularColor * SpecLight(input.normal,specDir,cameraDir,specularLight) + pointLight.PointLightColor * CalculatePointLight(input.normal, pointLDir, dist);
	
	return float4(output,1);
}