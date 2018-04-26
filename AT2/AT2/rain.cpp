#include "rain.h"

Rain::Rain()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
	m_model = 0;
	m_instanceBuffer = 0;
}

Rain::Rain(const Rain& other)
{

}

Rain::~Rain()
{

}

bool Rain::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result)
		return false;

	result = InitialiseBuffers(device);
	if (!result)
		return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
		return false;

	return true;

}

void Rain::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();

	ReleaseModel();

	return;
}

void Rain::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool Rain::InitialiseBuffers(ID3D11Device* device)
{
	RainVertex* vertices;
	InstanceType* instances;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;
	HRESULT result;

	m_vertexCount = 6;
	m_indexCount = 6;

	vertices = new RainVertex[m_vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	vertices[0].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].colour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	vertices[1].colour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);  // top right.
	vertices[2].colour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[3].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[3].colour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // top right.

	indices[3] = 3;  // Bottom left.
	indices[4] = 4;  // Top left.
	indices[5] = 5;  // top right.

	/*for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].pos = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		//vertices[i].tex = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//vertices[i].speed

		indices[i] = i;
	}*/

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(RainVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	m_maxInstanceCount = 10000;

	instances = new InstanceType[m_instanceCount];
	if (!instances)
		return false;

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_maxInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
		return false;

	delete[] instances;
	instances = 0;

	return true;
}

void Rain::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Rain::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(RainVertex);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 1;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

ID3D11ShaderResourceView* Rain::GetTexture()
{
	return m_texture->GetTexture();
}

bool Rain::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	//create the texture object.
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	//initialise the texture object.
	result = m_texture->Initialise(device, deviceContext, filename);
	if (!result)
		return false;

	return true;
}

void Rain::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

bool Rain::LoadModel(char* filename)
{
	std::ifstream fin;
	char input;
	int i;

	//open the model file
	fin.open(filename);

	//if it couldn't open the file, exit
	if (fin.fail())
		return false;

	//read up to the value of vertex count
	fin.get(input);
	while (input != ':')
		fin.get(input);

	//read in the vertex count
	fin >> m_vertexCount;

	//set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	//read up to the beginning of the data
	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin.get(input);
	fin.get(input);

	//read the vertex data
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	//close the model file
	fin.close();

	return true;
}

void Rain::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}

void Rain::Update(XMFLOAT3 camPos)
{
	//camPos = m_cam->GetPosition();
	//modelPos.x = 0.0f;
	//modelPos.y = 1.5f;
	//modelPos.z = 0.0f;

	//angle = atan2(modelPos.x - camPos.x, modelPos.z - camPos.z) * (180.0 / 3.1415);

	//rotation = (float)angle * 0.0174532925f;
	//worldMatrix = XMMatrixRotationY(rotation);

	//translateMatrix = XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);

	//worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);
}