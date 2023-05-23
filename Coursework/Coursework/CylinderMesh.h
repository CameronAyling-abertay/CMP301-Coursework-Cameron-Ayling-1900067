// Cylinder mesh built in the same way as the cube mesh
// Has an in-built size and position

#pragma once
#include "BaseMesh.h"

using namespace DirectX;

class CylinderMesh : public BaseMesh
{
public:
	// Constructors/destructor
	CylinderMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 resolution = XMFLOAT2(20, 5));
	CylinderMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 size, XMFLOAT3 pos, XMFLOAT2 resolution = XMFLOAT2(20, 5));
	~CylinderMesh();

protected:
	// Initialisation
	void initBuffers(ID3D11Device* device);
	void initBuffers(ID3D11Device* device, XMFLOAT2 size, XMFLOAT3 pos);

	// Variables
	XMFLOAT2 resolution;
};