struct PS_IN
{
	float4 position: SV_POSITION;
	float4 colour : COLOUR;
};

float4 PS_OUT(PS_IN input) : SV_TARGET
{
	return input.colour;
}

//float4 main(PS_IN input) : SV_TARGET
//{
//	return input.colour;
//}