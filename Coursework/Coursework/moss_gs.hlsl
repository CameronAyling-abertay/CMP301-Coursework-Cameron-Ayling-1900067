// triangle_gs
// Geometry shader that generates a triangle for every vertex.

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

[maxvertexcount(12)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

    // Top
    output.position = input[0].position + float4(0, .1, .1, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.5f, 0.0f);
    output.normal = mul(float3(0, 2, -4), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front left
    output.position = input[0].position + float4(-.1, -.1, 0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.0f, 1.0f);
    output.normal = mul(float3(0, 2, -4), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front right
    output.position = input[0].position + float4(.1, -.1, 0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(1.0f, 1.0f);
    output.normal = mul(float3(0, 2, -4), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    triStream.RestartStrip();

    // Top
    output.position = input[0].position + float4(0, .1, .1, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.5f, 0.0f);
    output.normal = mul(float3(-4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Back
    output.position = input[0].position + float4(0, -.1, .2, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.f, 1.0f);
    output.normal = mul(float3(-4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front left
    output.position = input[0].position + float4(-.1, -.1, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(1.0f, 1.0f);
    output.normal = mul(float3(-4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    triStream.RestartStrip();

    // Top
    output.position = input[0].position + float4(0, .1, .1, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.5f, 0.0f);
    output.normal = mul(float3(4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front right
    output.position = input[0].position + float4(.1, -.1, 0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(1.0f, 1.0f);
    output.normal = mul(float3(4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Back
    output.position = input[0].position + float4(0, -.1, .2, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.f, 1.0f);
    output.normal = mul(float3(4, 2, 2), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    triStream.RestartStrip();

    // Back
    output.position = input[0].position + float4(0, -.1, .2, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.5f, 0.0f);
    output.normal = mul(float3(0, -1, 0), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front right
    output.position = input[0].position + float4(.1, -.1, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(1.0f, 1.0f);
    output.normal = mul(float3(0, -1, 0), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    // Front left
    output.position = input[0].position + float4(-.1, -.1, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.depthPosition = output.position;
    output.tex = float2(0.0f, 1.0f);
    output.normal = mul(float3(0, -1, 0), (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(input[0].position, worldMatrix);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }
    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);
    triStream.Append(output);

    triStream.RestartStrip();
}