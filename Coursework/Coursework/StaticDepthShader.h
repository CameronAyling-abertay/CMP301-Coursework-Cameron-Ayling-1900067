#pragma once
#include "DXF.h"

class StaticDepthShader : public BaseShader
{
private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;

public:
	// Constructor/destructor
	StaticDepthShader(ID3D11Device* device, HWND hwnd);
	~StaticDepthShader();

	// Setting Shader Parameters
	// Each new line is a different buffer
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection);
};

