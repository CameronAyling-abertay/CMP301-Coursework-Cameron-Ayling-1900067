Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	return texture0.Sample(sampler0, input.tex);
}