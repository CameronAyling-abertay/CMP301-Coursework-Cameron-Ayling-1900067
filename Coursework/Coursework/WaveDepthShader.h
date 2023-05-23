#pragma once

#include "DXF.h"
#include "WaveShader.h"

using namespace std;
using namespace DirectX;

class WaveDepthShader : public BaseShader
{
private:

	struct TimeBufferType
	{
		float elapsedTime;
		XMFLOAT3 timePadding;
	};


	struct WaveParamBufferType
	{
		Wave waves[2];
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
	ID3D11Buffer* tessBuffer;

public:
	WaveDepthShader(ID3D11Device* device, HWND hwnd);
	~WaveDepthShader();

	// Setting Shader Parameters
	// Each new line is a different buffer
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
		float elapsedTime,
		Wave waves[2],
		XMINT4 edgeTess, XMINT2 inTess);
};