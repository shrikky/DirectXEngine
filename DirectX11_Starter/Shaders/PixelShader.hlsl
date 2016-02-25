
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	//float2 uv			: TEXCOORD;
};

struct DirectionalLight {
	float4 AmbientColor;		// 4
	float4 DiffuseColor;		// 4
	float3 Direction;			// 3

};
struct PointLight{
	float4 PointLightColor;		// 7
	float3 Position;			// 3
};
struct SpecularLight{
	float4 SpecularColor;		// 4			
	float SpecularStrength;		// 1
};
cbuffer externalData : register(b0)
{
	DirectionalLight directionLight, directionLight2;   
	PointLight pointLight;			
	SpecularLight specularLight;											
	float3 cameraPosition;								
};

// Directional Light calculation
float4 CalculateDirectionalLight(float3 normal, DirectionalLight light){
	float3 output;
	float3 direction = normalize(-light.Direction);
	normal = normalize(normal);
	float NdotL = saturate(dot(normal,light.Direction));
	output = light.DiffuseColor * NdotL;
	output += light.AmbientColor;
	return float4(output,1);

}

// PointLightCalculation
float CalculatePointLight(float3 normal, float3 direction, float dist){
	
	float point_NdotL;
	normal = normalize(normal);
	point_NdotL = saturate(dot(normal, direction));
	return point_NdotL;

}

///SpecularLight Calculation
float SpecLight(float3 normal, float3 camDir, float3 lightTowardsPLight, float strength){
	float spec;
	float3 reflection = reflect(-lightTowardsPLight, normal);
	spec = pow(max(dot(reflection, camDir),0),16);
	return spec * strength;

}
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 output;
	input.normal = normalize(input.normal);
	float dist = distance (pointLight.Position,input.worldPos);
	float3 dirTowardsPointLight = normalize(pointLight.Position - input.worldPos);  
	float3 dirTowardsCamera = normalize(cameraPosition - input.worldPos);
	output = 
	SpecLight(input.normal,dirTowardsCamera,dirTowardsPointLight,specularLight.SpecularStrength) +  // SpecularLight
	pointLight.PointLightColor * CalculatePointLight(input.normal, dirTowardsPointLight, dist) +	// PointLight
	CalculateDirectionalLight(input.normal, directionLight);										// DirectionalLight
	
	return float4(output,1);
}