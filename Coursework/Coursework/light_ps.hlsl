Texture2D texture0 : register(t0);
Texture2D shadowMaps[3] : register(t1);
SamplerState sampler0 : register(s0);
SamplerState sampler1 : register(s1);

struct Spotlight
{
	float4 diffuseColour;
	float4 ambientColour;
	float3 lightPosition;

	float maxAngle;
	float maxRange;
	float3 lightDirection;

	float3 attenuation;

	float specularPower;
	float4 specularColour;
};

struct Pointlight
{
	float4 diffuseColour;
	float4 ambientColour;
	float3 lightPosition;
	float padding1;

	float3 attenuation;

	float specularPower;
	float4 specularColour;
};

cbuffer LightBuffer : register(b0)
{
	Spotlight spotlights[3];
	Pointlight pointlights[4];
};

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

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = dot(normal, lightDirection);
	float4 colour = diffuse * intensity;
	return colour;
}

float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float specularPower)
{
	float3 halfway = normalize(lightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfway), 0.f), specularPower);
	return specularColour * specularIntensity;
}

// Shadow calcs
bool hasDepthData(float2 uv)
{
	if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1)
	{
		return false;
	}
	return true;
}

bool isInShadow(Texture2D map, float2 uv, float4 lightViewPos, float bias)
{
	float depthValue = map.Sample(sampler1, uv).r;

	float lightDepthValue = lightViewPos.z / lightViewPos.w;
	lightDepthValue -= bias;

	if (lightDepthValue < depthValue)
	{
		return false;
	}
	return true;
}

float2 getProjectiveCoords(float4 lightViewPos)
{
	float2 projTex = lightViewPos.xy / lightViewPos.w;
	projTex *= float2(0.5, -0.5);
	projTex += float2(0.5, 0.5);
	return projTex;
}

float4 main(InputType input) : SV_TARGET
{
	float shadowMapBias = 0.00025f;
	float4 textureColour = texture0.Sample(sampler0, input.tex);
	float4 lightColour = spotlights[0].ambientColour;
	float4 specColour = float4(0, 0, 0, 1.f);

	for (int i = 0; i < 3; i++)
	{
		float2 pTexCoord = getProjectiveCoords(input.lightViewPositions[i]);

		if (hasDepthData(pTexCoord) && !isInShadow(shadowMaps[i], pTexCoord, input.lightViewPositions[i], shadowMapBias))
		{
			float3 lightVector = spotlights[i].lightPosition - input.worldPosition;
			float dist = length(lightVector);
			lightVector = normalize(lightVector);

			float att = 1 / (spotlights[i].attenuation.x + spotlights[i].attenuation.y * dist + spotlights[i].attenuation.z * dist * dist);

			float3 lightDir = normalize(-spotlights[i].lightDirection);

			if (acos(dot(lightDir, lightVector)) < spotlights[i].maxAngle)
			{
				specColour += calcSpecular(lightVector, input.normal, input.viewVector, spotlights[i].specularColour, spotlights[i].specularPower);
				lightColour = lightColour + calculateLighting(lightDir, input.normal, spotlights[i].diffuseColour * att);
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		float3 lightVector = pointlights[i].lightPosition - input.worldPosition;
		float dist = length(lightVector);
		lightVector = normalize(lightVector);

		float att = 1 / (pointlights[i].attenuation.x + pointlights[i].attenuation.y * dist + pointlights[i].attenuation.z * dist * dist);

		specColour += calcSpecular(lightVector, input.normal, input.viewVector, pointlights[i].specularColour, pointlights[i].specularPower);
		lightColour = lightColour + calculateLighting(lightVector, input.normal, pointlights[i].diffuseColour * att);
	}

	lightColour.x = (lightColour.x < spotlights[0].ambientColour.x) ? spotlights[0].ambientColour.x : lightColour.x;
	lightColour.y = (lightColour.y < spotlights[0].ambientColour.y) ? spotlights[0].ambientColour.y : lightColour.y;
	lightColour.z = (lightColour.z < spotlights[0].ambientColour.z) ? spotlights[0].ambientColour.z : lightColour.z;

	return lightColour * textureColour + specColour;
}