cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[3];
	matrix lightProjectionMatrix[3];
};

cbuffer CameraBuffer : register(b1)
{
	float3 cameraPosition;
	float padding;
}

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	int distance : TEXCOORD1;
};

OutputType main(InputType input)
{
	OutputType output;

	output.position = input.position;
	output.tex = input.tex;
	output.normal = input.normal;

	float4 worldPos = mul(float4(input.position, 1.0), worldMatrix);
	output.distance = length(cameraPosition - worldPos);

    return output;
}