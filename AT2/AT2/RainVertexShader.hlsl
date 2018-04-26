Texture2D shaderTexture;
Texture2D positionTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


struct VertexInputType
{
	float4 position: POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	uint InstanceId : SV_InstanceID;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	uint InstanceId : SV_InstanceID;
};

PixelInputType RainVertexShader(VertexInputType input)
{
	PixelInputType output;

	float P = sqrt(128 * 128);
	//sample the position texture	
	uint v = uint(input.InstanceId / P);
	uint u = input.InstanceId - (v * P);

	float3 xyz = positionTexture.Load(uint(u,v)).xyz;

	input.position.xyz += xyz;

	input.position.w = 1.0f;
	
	output.position = input.position;

	//output.position = mul(input.position, worldMatrix);
	output.position = mul(input.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}