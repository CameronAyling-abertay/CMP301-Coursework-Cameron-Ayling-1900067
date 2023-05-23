#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

struct Wave
{
	float steepness;
	float wavelength;
	XMFLOAT2 direction;
};

class WaveShader : public BaseShader
{
private:

	struct SpotlightBuffer
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT3 position;

		float maxAngle;
		float maxRange;
		XMFLOAT3 direction;

		XMFLOAT3 attenuation;

		float specularPower;
		XMFLOAT4 specular;
	};

	struct PointlightBuffer
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT3 position;
		float padding1;

		XMFLOAT3 attenuation;
		float specularPower;

		XMFLOAT4 specular;
	};

	struct TimeBufferType
	{
		float elapsedTime;
		XMFLOAT3 timePadding;
	};


	struct WaveParamBufferType
	{
		Wave waves[2];
	};

	struct LightBufferType
	{
		SpotlightBuffer spotlights[3];
		PointlightBuffer pointlights[4];
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

	struct TessBufferType
	{
		XMINT4 edgeTess;
		XMINT2 inTess;
		XMFLOAT2 padding;
	};

	void initShader(const wchar_t* vs, const wchar_t* ps);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* waveParamBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* tessBuffer;

	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;

public:
	WaveShader(ID3D11Device* device, HWND hwnd);
	~WaveShader();

	// Setting Shader Parameters
	// Each new line is a different buffer
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
		ID3D11ShaderResourceView* texture,
		float elapsedTime,
		Wave waves[2],
		Light* spotlights[3],
		Light* pointlights[4],
		Camera* camera,
		ID3D11ShaderResourceView* depthMaps[3],
		XMINT4 edgeTess, XMINT2 inTess);
};