// Upgraded cube mesh
// Has an in-built size and position

#pragma once
#include "BaseMesh.h"

using namespace DirectX;

class CubeMesh2 : public BaseMesh
{
public:
	// Constructors/destructor
	CubeMesh2(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 20);
	CubeMesh2(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT3 size, XMFLOAT3 pos, int resolution = 20);
	~CubeMesh2();

protected:
	// Initialisation
	void initBuffers(ID3D11Device* device);
	void initBuffers(ID3D11Device* device, XMFLOAT3 size, XMFLOAT3 pos);

	// Variables
	int resolution;
};