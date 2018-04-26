struct PixelInputType
{
	float4 position : SV_POSITION;

};

float4 RainPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;

	color.x = input.position.x;
	color.y = input.position.y - 0.005f;
	color.z = input.position.z;
	if (color.y <= 0)
	{
		color.y = 100.0f;
	}

	return color;
}

