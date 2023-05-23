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
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPosition : TEXCOORD1;
	float3 viewVector : TEXCOORD2;
	float4 depthPosition : TEXCOORD3;
	float4 lightViewPositions[3] : TEXCOORD4;
};

OutputType main(InputType input)
{
	OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);

	output.worldPosition = output.position;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	for (int i = 0; i < 3; i++)
	{
		output.lightViewPositions[i] = mul(input.position, worldMatrix);
		output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
		output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
	}

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	
	output.viewVector = cameraPosition - output.worldPosition;
	output.viewVector = normalize(output.viewVector);

	output.depthPosition = output.position;

	return output;
}