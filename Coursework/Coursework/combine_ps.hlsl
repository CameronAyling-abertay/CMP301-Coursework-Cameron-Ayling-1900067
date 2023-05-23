Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float3 outColour = texture0.Sample(sampler0, input.tex);

	float3 mod = texture1.Sample(sampler0, input.tex);
	mod.x = (mod.x < 0) ? 0 : mod.x;
	mod.y = (mod.y < 0) ? 0 : mod.y;
	mod.z = (mod.z < 0) ? 0 : mod.z;

	outColour += mod;

	return float4(outColour, 1);
}