Texture2D randomTexture1;
Texture2D randomTexture2;
Texture2D rainTexture;
SamplerState SampleType;


struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 randomPosition : TEXTURE0;
};

//struct PixelOutputType

float4 RandomPixelShader(PixelInputType input) : SV_TARGET
{
	float3 pos;
	float4 color;


	input.color.y -= 1.0f * 0.0005f;
	if (input.color.g <= 0)
	{
		input.color.g = 150;
	}



	return color;
}