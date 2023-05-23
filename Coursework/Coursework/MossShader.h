// Shader to created triangles of moss-like geometry

#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

class MossShader : public BaseShader
{
private:
	// Container to hold spotlight variables for easier buffer organisation
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

	// Container to hold point light variables for easier buffer organisation
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

	// Buffer to hold the lights passed in
	struct LightBufferType
	{
		SpotlightBuffer spotlights[3];
		PointlightBuffer pointlights[4];
	};

	// Buffer to hold the camera position
	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

	// Initialisation
	void initShader(const wchar_t* vs, const wchar_t* ps);
	void initShader(const wchar_t* vsFilename, const wchar_t* gsFilename, const wchar_t* psFilename);

	// Buffers and samplers
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;

public:
	// Constructor/destructor
	MossShader(ID3D11Device* device, HWND hwnd);
	~MossShader();

	// Setting Shader Parameters
	// Each new line is a different buffer
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture,
		Light* spotlights[3],
		Light* pointlights[4],
		Camera* camera,
		ID3D11ShaderResourceView* depthMaps[3]);
};
