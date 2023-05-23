#include "CylinderMesh.h"

// Initialise vertex data, buffers and load texture.
CylinderMesh::CylinderMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 size, XMFLOAT3 pos, XMFLOAT2 lresolution)
{
	resolution = lresolution;
	initBuffers(device, size, pos);
}

CylinderMesh::CylinderMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 lresolution)
{
	resolution = lresolution;
	initBuffers(device);
}

CylinderMesh::~CylinderMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

void CylinderMesh::initBuffers(ID3D11Device* device)
{
}

// Initialise geometry buffers (vertex and index).
// Generate and store cube vertices, normals and texture coordinates
void CylinderMesh::initBuffers(ID3D11Device* device, XMFLOAT2 size, XMFLOAT3 pos)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Count setup
	vertexCount = resolution.x * 6 * (resolution.y + 1);

	indexCount = vertexCount;

	// Create the vertex and index array.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	//Angle setup for tracking and incrementation
	double theta = 0;
	double thetaInc = 6.2832 / resolution.x;

	//Array for samples taken at each increment of the angle horizontally - better than doing the maths each time
	XMFLOAT3 samples[50]{ XMFLOAT3(0, 0, 0) };

	//Generating the samples
	for (int i = 0; i < resolution.x + 1; i++)
	{
		samples[i] = XMFLOAT3(cos(theta), 0, sin(theta));
		theta += thetaInc;
	}

	int v = 0;
	int i = 0;

	// Top face
	for (int face = 0; face < resolution.x; face++)
	{
		vertices[v].position = XMFLOAT3(samples[face].x * size.x, size.y, samples[face].z * size.x);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, 1, 0);

		indices[i] = i;
		v++;
		i++;

		vertices[v].position = XMFLOAT3(samples[face + 1].x * size.x, size.y, samples[face + 1].z * size.x);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, 1, 0);

		indices[i] = i;
		v++;
		i++;

		vertices[v].position = XMFLOAT3(0, size.y, 0);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, 1, 0);

		indices[i] = i;
		v++;
		i++;
	}

	// Bottom face
	for (int face = 0; face < resolution.x; face++)
	{
		vertices[v].position = XMFLOAT3(samples[face].x * size.x, 0, samples[face].z * size.x);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, -1, 0);

		indices[i] = i;
		v++;
		i++;

		vertices[v].position = XMFLOAT3(0, 0, 0);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, -1, 0);

		indices[i] = i;
		v++;
		i++;

		vertices[v].position = XMFLOAT3(samples[face + 1].x * size.x, 0, samples[face + 1].z * size.x);
		vertices[v].position.x += pos.x;
		vertices[v].position.y += pos.y;
		vertices[v].position.z += pos.z;
		vertices[v].normal = XMFLOAT3(0, -1, 0);

		indices[i] = i;
		v++;
		i++;
	}

	// Wraparound plane
	for (int line = 0; line < resolution.y; line++)
	{
		for (int face = 0; face < resolution.x; face++)
		{
			vertices[v].position = XMFLOAT3(samples[face].x * size.x, line * size.y / resolution.y, samples[face].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2(face / resolution.x, 1 - line / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face].x, 0, samples[face].y);

			indices[i] = i;
			v++;
			i++;

			vertices[v].position = XMFLOAT3(samples[face + 1].x * size.x, line * size.y / resolution.y, samples[face + 1].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2((face + 1) / resolution.x, 1 - line / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face + 1].x, 0, samples[face + 1].y);

			indices[i] = i;
			v++;
			i++;

			vertices[v].position = XMFLOAT3(samples[face].x * size.x, (line + 1) * size.y / resolution.y, samples[face].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2(face / resolution.x, 1 - (line + 1) / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face].x, 0, samples[face].y);

			indices[i] = i;
			v++;
			i++;

			vertices[v].position = XMFLOAT3(samples[face].x * size.x, (line + 1) * size.y / resolution.y, samples[face].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2(face / resolution.x, 1 - (line + 1) / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face].x, 0, samples[face].y);

			indices[i] = i;
			v++;
			i++;

			vertices[v].position = XMFLOAT3(samples[face + 1].x * size.x, line * size.y / resolution.y, samples[face + 1].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2((face + 1) / resolution.x, 1 - line / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face + 1].x, 0, samples[face + 1].y);

			indices[i] = i;
			v++;
			i++;

			vertices[v].position = XMFLOAT3(samples[face + 1].x * size.x, (line + 1) * size.y / resolution.y, samples[face + 1].z * size.x);
			vertices[v].position.x += pos.x;
			vertices[v].position.y += pos.y;
			vertices[v].position.z += pos.z;
			vertices[v].texture = XMFLOAT2((face + 1) / resolution.x, 1 - (line + 1) / resolution.y);
			vertices[v].normal = XMFLOAT3(samples[face + 1].x, 0, samples[face + 1].y);

			indices[i] = i;
			v++;
			i++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
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