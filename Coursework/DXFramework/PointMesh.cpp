// Point mesh.
// For geometry shader demonstration.
// Note sendData() override.
#include "pointmesh.h"

// Initialise buffers and load texture.
PointMesh::PointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
PointMesh::~PointMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

// Generate point mesh. Simple triangle.
void PointMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 55;
	indexCount = 55;


	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data.
	for (int i = 0; i < 10; i++)
	{
		vertices[i].position.x = -0.45 + i * 0.1;
		vertices[i].position.y = 0;
		vertices[i].position.z = 0;
	}

	for (int i = 0; i < 9; i++)
	{
		vertices[i + 10].position.x = -0.4 + i * 0.1;
		vertices[i + 10].position.y = -.05;
		vertices[i + 10].position.z = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		vertices[i + 19].position.x = -0.35 + i * 0.1;
		vertices[i + 19].position.y = -.1;
		vertices[i + 19].position.z = 0;
	}

	for (int i = 0; i < 7; i++)
	{
		vertices[i + 27].position.x = -0.3 + i * 0.1;
		vertices[i + 27].position.y = -.15;
		vertices[i + 27].position.z = 0;
	}

	for (int i = 0; i < 6; i++)
	{
		vertices[i + 34].position.x = -0.25 + i * 0.1;
		vertices[i + 34].position.y = -.2;
		vertices[i + 34].position.z = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		vertices[i + 40].position.x = -0.2 + i * 0.1;
		vertices[i + 40].position.y = -.25;
		vertices[i + 40].position.z = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		vertices[i + 45].position.x = -0.15 + i * 0.1;
		vertices[i + 45].position.y = -.3;
		vertices[i + 45].position.z = 0;
	}

	for (int i = 0; i < 3; i++)
	{
		vertices[i + 49].position.x = -0.1 + i * 0.1;
		vertices[i + 49].position.y = -.35;
		vertices[i + 49].position.z = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		vertices[i + 52].position.x = -0.05 + i * 0.1;
		vertices[i + 52].position.y = -.4;
		vertices[i + 52].position.z = 0;
	}

	vertices[54].position.x = 0;
	vertices[54].position.y = -.45;
	vertices[54].position.z = 0;

	// Load the index array with data.
	for (int i = 0; i < 55; i++)
	{
		indices[i] = i;
	}
	

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

// Override sendData()
// Change in primitive topology (pointlist instead of trianglelist) for geometry shader use.
void PointMesh::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}

