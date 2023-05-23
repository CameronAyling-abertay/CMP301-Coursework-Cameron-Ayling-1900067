// tessellation shader.cpp
#include "waveshader.h"


WaveShader::WaveShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tess_vs.cso", L"waves_hs.cso", L"waves_ds.cso", L"light_ps.cso");
}


WaveShader::~WaveShader()
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

void WaveShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
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

	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

	// Light buffer setup
	D3D11_BUFFER_DESC lightBufferDesc;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	D3D11_BUFFER_DESC tessBufferDesc;
	tessBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessBufferDesc.ByteWidth = sizeof(TessBufferType);
	tessBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessBufferDesc.MiscFlags = 0;
	tessBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&tessBufferDesc, NULL, &tessBuffer);

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 1;
	samplerDesc.BorderColor[2] = 1;
	samplerDesc.BorderColor[3] = 1;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);
}

void WaveShader::initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}


void WaveShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
	ID3D11ShaderResourceView* texture,
	float elapsedTime,
	Wave waves[2],
	Light* spotlights[3],
	Light* pointlights[4],
	Camera* camera,
	ID3D11ShaderResourceView* depthMaps[3],
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

	for (int i = 0; i < 3; i++)
	{
		XMMATRIX lview = XMMatrixTranspose(spotlights[i]->getViewMatrix());
		XMMATRIX lproj = XMMatrixTranspose(spotlights[i]->getProjectionMatrix());
		dataPtr->lightviewMatrix[i] = lview;
		dataPtr->lightProjectionMatrix[i] = lproj;
	}

	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

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

	// Camera Buffer
	CameraBufferType* cameraPtr;
	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->position = camera->getPosition();
	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->DSSetConstantBuffers(3, 1, &cameraBuffer);
	deviceContext->VSSetConstantBuffers(1, 1, &cameraBuffer);

	// Light Buffer
	LightBufferType* lightPtr;

	result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;

	for (int i = 0; i < 3; i++)
	{
		lightPtr->spotlights[i].diffuse = spotlights[i]->getDiffuseColour();
		lightPtr->spotlights[i].position = spotlights[i]->getPosition();
		lightPtr->spotlights[i].ambient = spotlights[i]->getAmbientColour();
		lightPtr->spotlights[i].maxRange = 1000.0f;
		lightPtr->spotlights[i].direction = spotlights[i]->getDirection();
		lightPtr->spotlights[i].attenuation = { 1, .1, 0.01f };
		lightPtr->spotlights[i].specularPower = spotlights[i]->getSpecularPower();
		lightPtr->spotlights[i].specular = spotlights[i]->getSpecularColour();
	}

	lightPtr->spotlights[0].maxAngle = 0.55f;
	lightPtr->spotlights[1].maxAngle = 0.5f;
	lightPtr->spotlights[2].maxAngle = 0.5f;

	for (int i = 0; i < 4; i++)
	{
		lightPtr->pointlights[i].diffuse = pointlights[i]->getDiffuseColour();
		lightPtr->pointlights[i].ambient = pointlights[i]->getAmbientColour();
		lightPtr->pointlights[i].position = pointlights[i]->getPosition();
		lightPtr->pointlights[i].attenuation = { 1, .25, .01f };
		lightPtr->pointlights[i].specularPower = pointlights[i]->getSpecularPower();
		lightPtr->pointlights[i].specular = pointlights[i]->getSpecularColour();
	}

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	TessBufferType* tessPtr;
	deviceContext->Map(tessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	tessPtr = (TessBufferType*)mappedResource.pData;
	tessPtr->edgeTess = edgeTess;
	tessPtr->inTess = inTess;
	deviceContext->Unmap(tessBuffer, 0);
	deviceContext->HSSetConstantBuffers(0, 1, &tessBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &(depthMaps[0]));
	deviceContext->PSSetShaderResources(2, 1, &(depthMaps[1]));
	deviceContext->PSSetShaderResources(3, 1, &(depthMaps[2]));
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);
}