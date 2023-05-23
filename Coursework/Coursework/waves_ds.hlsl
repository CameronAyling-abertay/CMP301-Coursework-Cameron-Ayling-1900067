// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[3];
    matrix lightProjectionMatrix[3];
};

cbuffer TimeBuffer : register(b1)
{
    float elapsedTime;
    float3 timePadding;
}

struct Wave
{
    float steepness;
    float wavelength;
    float2 direction;
};

cbuffer WaveParamBuffer : register(b2)
{
    Wave waves[2];
}

cbuffer CameraBuffer : register(b3)
{
    float3 cameraPosition;
    float padding;
}

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
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

float3 genWave(Wave wave, float3 pos, inout float3 tangent, inout float3 binormal)
{
    // Defining Pi
    static const float PI = 2 * asin(1);

    // Deconstructing the wave
    float steepness = wave.steepness;
    float wavelength = wave.wavelength;
    float2 direction = wave.direction;

    // Create Gerstner effect
    float phase = 2 * PI / wavelength;
    float2 dir = normalize(direction);

    float period = sqrt(9.8f / phase);
    float angle = phase * (dot(dir, pos.xz) - period * elapsedTime);
    float amp = steepness / phase;

    // Normal calculation
    tangent += float3(
        dir.x * dir.x * steepness * -sin(angle),
        dir.x * steepness * cos(angle),
        dir.y * dir.x * steepness * -sin(angle)
        );

    binormal += float3(
        dir.x * dir.y * steepness * -sin(angle),
        dir.y * steepness * cos(angle),
        dir.y * dir.y * steepness * -sin(angle)
        );

    // Wave height return
    return float3(
        dir.x * amp * cos(angle),
        amp * sin(angle),
        dir.y * amp * cos(angle)
        );
}

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition;
    float2 texCoord;
    OutputType output;

    float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
    float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    vertexPosition = lerp(v1, v2, uvwCoord.x);

    // Vertex manipulation

    float3 pos = vertexPosition;
    float3 tangent = float3(1, 0, 0);
    float3 binormal = float3(0, 0, 1);
    float3 newPos = pos;

    newPos += genWave(waves[0], pos, tangent, binormal);
    newPos += genWave(waves[1], pos, tangent, binormal);

    vertexPosition.xyz = newPos;

    float3 normal = normalize(cross(binormal, tangent));

    output.normal = normal;

    if (output.normal.y < 0.f)
    {
        output.normal *= -1.f;
    }

    output.normal = mul(output.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);

    output.worldPosition = output.position;

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    for (int i = 0; i < 3; i++)
    {
        output.lightViewPositions[i] = mul(output.worldPosition, lightViewMatrix[i]);
        output.lightViewPositions[i] = mul(output.lightViewPositions[i], lightProjectionMatrix[i]);
    }

    float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
    float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);
    texCoord = lerp(t1, t2, uvwCoord.x);

    output.tex = texCoord;

    output.viewVector = cameraPosition - output.worldPosition;
    output.viewVector = normalize(output.viewVector);

    output.depthPosition = output.position;

    return output;
}