cbuffer matrixbuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_IN
{
	float3 position : POSITION;
	float4 colour : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 colour : COLOUR;
};


VS_OUT LineVertexShader(float3 position : POSITION, float4 colour : COLOUR) : SV_POSITION
{
	VS_OUT output;

	//output.pos = mul(position, worldMatrix);
	//output.pos = mul(output.pos, viewMatrix);
	//output.pos = mul(output.pos, projectionMatrix);

	output.pos = mul(float4(position, 1.0), mul(mul(worldMatrix, viewMatrix), projectionMatrix));
	
	output.colour = colour;
	return output;
}

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}