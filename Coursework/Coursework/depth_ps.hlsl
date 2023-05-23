struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPosition : TEXCOORD1;
	float3 viewVector : TEXCOORD2;
	float4 depthPosition : TEXCOORD3;
	float4 lightViewPositions[3] : TEXCOORD4;
};

float4 main(InputType input) : SV_TARGET
{
	float depthValue;

	depthValue = input.depthPosition.z / input.depthPosition.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}