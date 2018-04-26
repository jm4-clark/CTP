#include "LineClass.h"



LineClass::LineClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;
	m_vertexCountPerHair = 1;
	m_indexCountPerHair = 1;
	m_parentCount = 0;
	m_physicsOn = true;
}

LineClass::LineClass(std::vector<XMFLOAT3> parent_pos, std::vector<XMFLOAT3> parent_norm, int parent_count, int length)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_instanceBuffer = 0;
	m_vertexCountPerHair = 3;
	m_indexCountPerHair = 1;
	m_physicsOn = true;
	m_parentCount = parent_count;
	m_parentList.resize(parent_count);
	m_numOfHairs = length;
	m_lengthOfHair = 2;
	//InterpolateParents(parent_pos, parent_norm);
	for (int i = 0; i < m_parentCount; i++)
	{
		m_parentList[i].Pos = parent_pos[i];
		m_parentList[i].Normal = parent_norm[i];
		m_parentList[i].Tex = XMFLOAT2(0, 0);
	}

	//parent.Pos = parent_pos;
	//parent.Normal = parent_norm;
	//parent.Tex = XMFLOAT2(0, 0);

	//for (int i = 0; i < length; i++)
	//{
	//	XMFLOAT3 pos;
	//	XMFLOAT3 norm = parent_norm;
	//	if (i == 0)
	//	{
	//		pos = parent_pos;
	//		norm = parent_norm;
	//	}
	//	else 
	//	{
	//		pos.x = m_strands[i - 1].Pos.x + (m_strands[i - 1].norm.x * 20);
	//		pos.y = m_strands[i - 1].Pos.y + (m_strands[i - 1].norm.y * 20);
	//		pos.z = m_strands[i - 1].Pos.z + (m_strands[i - 1].norm.z * 20);
	//		norm = parent_norm;
	//	}
	//	PointType point;
	//	point.Pos = pos;
	//	point.norm = norm;
	//	//LineClass* newHair = new LineClass(pos,norm);
	//	m_strands.push_back(point);
	//}

}

LineClass::LineClass(XMFLOAT3 parent_pos, XMFLOAT3 parent_norm)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_vertexCountPerHair = 1;
	m_indexCountPerHair = 1;
	m_parentCount = 0;
	m_physicsOn = true;
}

LineClass::LineClass(const LineClass& other)
{
}


LineClass::~LineClass()
{
}

bool LineClass::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = InitialiseBuffers(device);
	if (!result)
		return false;

	return true;
}

void LineClass::Shutdown()
{
	ShutdownBuffers();

	return;
}

void LineClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

bool LineClass::InitialiseBuffers(ID3D11Device* device) 
{

	//for (int i = 0; i < m_strands.size(); i++)
	//{
	//	//Are we neglecting the normal? Are we using vertex and indices wrong
	//	Basic32 v;
	//	v.Pos = m_strands.at(i).Pos;
	//	m_vertex.push_back(v);
	//}


	//int parentCount = sizeof(parent) / sizeof(parent[0]);
	//PointType* vertices;
	int lineCount = sizeof(m_parentList) / sizeof(m_parentList[0]);
	unsigned long* indices;
	
	
	
	//m_numOfHairs = m_numOfHairs * 2;
	HRESULT result;

	
	//if (!vertices)
		//return false;

	indices = new unsigned long[m_indexCountPerHair * m_parentCount];
	if (!indices)
		return false;
	int c = 0;
	float offsetX, offsetY, offsetZ;
	offsetX =-1.5f;
	offsetY = -8.0f;
	offsetZ = -2.0f;
	for (int i = 0; i < m_parentCount; i++)
	{
		for (int j = 0; j < m_numOfHairs; j++)
		{

			if (j == 0)
			{
				PointType pt;
				//base vert lol
				pt.Pos.x = m_parentList[i].Pos.x + offsetX;
				pt.Pos.y = m_parentList[i].Pos.y + offsetY;
				pt.Pos.z = m_parentList[i].Pos.z + offsetZ;
				pt.norm = m_parentList[i].Normal;
				vertices.push_back(pt);
			}
			if (j != 0 && j % 2 == 0)
			{
				//new start
				PointType pstart;

				pstart.Pos.x = vertices.at(c - 1).Pos.x;
				pstart.Pos.y = vertices.at(c - 1).Pos.y;
				pstart.Pos.z = vertices.at(c - 1).Pos.z;
				pstart.norm = vertices.at(c - 1).norm;
				vertices.push_back(pstart);

			}

			if (j != 0 && j % 2 == 1)
			{
				//new end
				PointType ptend;

				ptend.Pos.x = vertices.at(c - 1).Pos.x + (vertices.at(c - 1).norm.x);// *(float)j * 2);//Change multiple for length(float)j
				ptend.Pos.y = vertices.at(c - 1).Pos.y + (vertices.at(c - 1).norm.y);// *(float)j * 2);
				ptend.Pos.z = vertices.at(c - 1).Pos.z + (vertices.at(c - 1).norm.z * (float)j * 2);

				ptend.norm = vertices.at(c - 1).norm;
				vertices.push_back(ptend);
			}
			c++;
			//vertices[(i * m_vertexCountPerHair) + j].Pos = XMFLOAT3(posX, posY, posZ);+ parentList[i].Normal.x * (float)j
			//vertices[(i * m_vertexCountPerHair) + j].Colour = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			//indices[(i*m_vertexCountPerHair) + j] = j;
		}
	}

/*
	for (int i = 0; i < m_parentCount; i++)
	{
		for (int j = 0; j < m_vertexCountPerHair; i++)
		{
			float posX = parent[i].Pos.x + parent[i].Norm.x * (float)j;
			float posY = parent[i].Pos.y + parent[i].Norm.y * (float)j;
			float posZ = parent[i].Pos.z + parent[i].Norm.z * (float)j;

			vertices[(i * m_vertexCountPerHair) + j].Pos = XMFLOAT3(posX, posY, posZ);
			vertices[(i * m_vertexCountPerHair) + j].Colour = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			indices[(i*m_vertexCountPerHair) + j] = i;
		}* m_vertexCountPerHair * m_parentCount * m_indexCountPerHair * m_parentCount
	}*/
	//vertex buffer stuff
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(PointType) * m_vertexCountPerHair * m_parentCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0; 

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &vertices[0];


//	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	//if (FAILED(result))
	//	return false;


	//D3D11_BUFFER_DESC  indexBufferDesc;
	////index buffer stuff
	//indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//indexBufferDesc.ByteWidth = sizeof(Basic32) * vertices.size();
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;


	////Possibly the issue here, we need index of points between line?
	//D3D11_SUBRESOURCE_DATA  indexData;
	//indexData.pSysMem = &vertices;


	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	//if (FAILED(result))
	//	return false;

	//delete vertices;
	//vertices = 0;

	/*delete[] indices;
	indices = 0;
*/

	m_instanceCount = m_parentCount;

	instanceDataVector.reserve(m_instanceCount);
	instanceDataVector.resize(m_instanceCount);

	c = 0;
	for (int i = 0; i < m_parentCount; i++)
	{
		for (int j = 0; j < m_numOfHairs; j++)
		{

			if (j == 0)
			{
				PointType pt;
				//base vert lol
				pt.Pos.x = m_parentList[i].Pos.x;
				pt.Pos.y = m_parentList[i].Pos.y;
				pt.Pos.z = m_parentList[i].Pos.z;
				pt.norm = m_parentList[i].Normal; 
				instanceDataVector[i].Pos = pt.Pos;
			}
			if (j != 0 && j % 2 == 0)
			{
				//new start
				PointType pstart;

				pstart.Pos.x = vertices.at(c - 1).Pos.x;
				pstart.Pos.y = vertices.at(c - 1).Pos.y;
				pstart.Pos.z = vertices.at(c - 1).Pos.z;
				pstart.norm = vertices.at(c - 1).norm;
				instanceDataVector[i].Pos = pstart.Pos;

			}

			if (j != 0 && j % 2 == 1)
			{
				//new end
				PointType ptend;

				ptend.Pos.x = vertices.at(c - 1).Pos.x + (vertices.at(c - 1).norm.x);// *(float)j * 2);//Change multiple for length(float)j
				ptend.Pos.y = vertices.at(c - 1).Pos.y + (vertices.at(c - 1).norm.y);// *(float)j * 2);
				ptend.Pos.z = vertices.at(c - 1).Pos.z + (vertices.at(c - 1).norm.z);// *(float)j * 2);

				ptend.norm = vertices.at(c - 1).norm;
				instanceDataVector[i].Pos = ptend.Pos;
			}
			c++;
			//vertices[(i * m_vertexCountPerHair) + j].Pos = XMFLOAT3(posX, posY, posZ);+ parentList[i].Normal.x * (float)j
			//vertices[(i * m_vertexCountPerHair) + j].Colour = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			//indices[(i*m_vertexCountPerHair) + j] = j;
		}
	}

	instanceDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceDesc.CPUAccessFlags = 0;
	instanceDesc.MiscFlags = 0;

	instanceData.pSysMem = &instanceDataVector[0];
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&instanceDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
		return false;

	return true;
}

void LineClass::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void LineClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	/*unsigned int stride;
	unsigned int offset;

	stride = sizeof(PointType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//deviceContext->DrawIndexed(m_strands.size(), 0, 0);
	deviceContext->Draw(vertices.size(), 0);*/
	
	m_instanceBuffer->Release();
	m_instanceBuffer = nullptr;
	ID3D11Device* temp;
	deviceContext->GetDevice(&temp);
	temp->CreateBuffer(&instanceDesc, &instanceData, &m_instanceBuffer);
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set vertex buffer stride and offset.
	strides[0] = sizeof(PointType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 1;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	deviceContext->DrawInstanced(m_numOfHairs, m_instanceCount, 0, 0);

	return;
}

void LineClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, float delta_time)
{
	XMFLOAT3 pos;
	XMFLOAT3 minusOnePos;
	XMFLOAT3 diff;

	/*for (int i = 0; i < m_parentCount; i++)
	{
		for (int j = 0; j < m_vertexCountPerHair; j++)
		{
			pos = m_vertices[i*m_vertexCountPerHair + j].pos;
			minusOnePos = vertices[i*m_vertexCountPerHair + j - 1].pos;

			if (m_physicsOn)
			{
				pos.y -= 0.1f * delta_time;

				diff = pos - minusOnePos;

				float distSqaured = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
				if(distSqaured > (m_hairLength / m_vertexCountPerHair))
			}
		}
	}*/
}