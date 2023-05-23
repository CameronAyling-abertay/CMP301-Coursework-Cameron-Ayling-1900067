#include "BaseMesh.h"

using namespace DirectX;

class CubeMesh : public BaseMesh
{

public:
	CubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float size, XMFLOAT3 pos, int resolution = 20);
	~CubeMesh();

protected:
	void initBuffers(ID3D11Device* device, float size, XMFLOAT3 pos);
	int resolution;
};