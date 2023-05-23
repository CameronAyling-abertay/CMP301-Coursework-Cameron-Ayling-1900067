// tessellation shader.cpp
#include "wavedepthshader.h"


WaveDepthShader::WaveDepthShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tess_vs.cso", L"waves_hs.cso", L"waves_ds.cso", L"depth_ps.cso");
}


WaveDepthShader::~WaveDepthShader()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void WaveDepthShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Time buffer setup
	D3D11_BUFFER_DESC timeBufferDesc;

	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer);

	// Wave parameter buffer setup
	D3D11_BUFFER_DESC waveParamBufferDesc;

	waveParamBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	waveParamBufferDesc.ByteWidth = sizeof(WaveParamBufferType);
	waveParamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waveParamBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waveParamBufferDesc.MiscFlags = 0;
	waveParamBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&waveParamBufferDesc, NULL, &waveParamBuffer);

	D3D11_BUFFER_DESC tessBufferDesc;
	tessBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessBufferDesc.ByteWidth = sizeof(TessBufferType);
	tessBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessBufferDesc.MiscFlags = 0;
	tessBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&tessBufferDesc, NULL, &tessBuffer);
}

void WaveDepthShader::initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}


void WaveDepthShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
	float elapsedTime,
	Wave waves[2],
	XMINT4 edgeTess, XMINT2 inTess)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(world);
	XMMATRIX tview = XMMatrixTranspose(view);
	XMMATRIX tproj = XMMatrixTranspose(projection);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);

	// Time Buffer
	TimeBufferType* timePtr;

	result = deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	timePtr->elapsedTime = elapsedTime;
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->DSSetConstantBuffers(1, 1, &timeBuffer);

	// Wave Parameter Buffer
	WaveParamBufferType* waveParamPtr;

	result = deviceContext->Map(waveParamBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waveParamPtr = (WaveParamBufferType*)mappedResource.pData;
	waveParamPtr->waves[0] = waves[0];
	waveParamPtr->waves[1] = waves[1];
	deviceContext->Unmap(waveParamBuffer, 0);
	deviceContext->DSSetConstantBuffers(2, 1, &waveParamBuffer);

	// Tessellation Buffer
	TessBufferType* tessPtr;
	deviceContext->Map(tessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	tessPtr = (TessBufferType*)mappedResource.pData;
	tessPtr->edgeTess = edgeTess;
	tessPtr->inTess = inTess;
	deviceContext->Unmap(tessBuffer, 0);
	deviceContext->HSSetConstantBuffers(0, 1, &tessBuffer);
}