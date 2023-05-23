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
	float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };

	float width;
	float height;
	texture0.GetDimensions(width, height);

	float2 offset = (1 / width, 1 / height);

	float3 outColour = float3(0, 0, 0);

	float3 mod;

	for (int i = 0; i < 5; i++)
	{
		mod = texture0.Sample(sampler0, input.tex + float2(offset.x * i, 0)).xyz - float3(1, 1, 1);
		mod.x = (mod.x < 0) ? 0 : mod.x;
		mod.y = (mod.y < 0) ? 0 : mod.y;
		mod.z = (mod.z < 0) ? 0 : mod.z;
		outColour += mod * weight[i];

		mod = texture0.Sample(sampler0, input.tex - float2(offset.x * i, 0)).xyz - float3(1, 1, 1);
		mod.x = (mod.x < 0) ? 0 : mod.x;
		mod.y = (mod.y < 0) ? 0 : mod.y;
		mod.z = (mod.z < 0) ? 0 : mod.z;
		outColour += mod * weight[i];
	}

	return float4(outColour, 1.0f);
}