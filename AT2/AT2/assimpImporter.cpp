#include "assimpImporter.h"



AssimpImporter::AssimpImporter()
{
	pScene = 0;
	pMesh = 0;
	m_numIndices = 0;
	m_textureShader = 0;
	// Create the texture shader object.
	m_textureShader = new TextureShaderClass;
	
	/*if (!m_textureShader)
		return false;*/
	
}


AssimpImporter::~AssimpImporter()
{
	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}
}



bool AssimpImporter::LoadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const char* fileName, char* textureFileName, bool hasHair)
{
	UINT m_numVertices;
	UINT m_numFaces;

	bool result;

	pScene = imp.ReadFile(fileName, 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices);

	if (!pScene)
	{
		MessageBox(NULL, "Error reading model file", "Error", MB_OK);
		return false;
	}
	
	pMesh = pScene->mMeshes[0];
	if(!pMesh)
	{
		MessageBox(NULL, "Failed to find meshes", "Error", MB_OK);
		return false;
	}
	m_numMeshes = pScene->mNumMeshes;
	
	std::vector<XMFLOAT3> positions, normals;
	std::vector<Basic32> vertices;
	std::vector<DWORD> mIndicies;
	std::vector<XMFLOAT3> lineStarts;
	std::vector<XMFLOAT3> lineDir;
	for (int i = 0; i < m_numMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[i];
		//m_hairs.size(sizeof(mesh->mNumVertices));
		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			Basic32 v;
			XMFLOAT3 lineStartPos;
			XMFLOAT3 lineDirection;

			lineStartPos.x = mesh->mVertices[j].x;
			lineStartPos.y = mesh->mVertices[j].y;
			lineStartPos.z = mesh->mVertices[j].z;

			lineStarts.push_back(lineStartPos);
			
			XMFLOAT3 pos;
			pos.x = mesh->mVertices[j].x;
			pos.y = mesh->mVertices[j].y;
			pos.z = mesh->mVertices[j].z;

			v.Pos.x = mesh->mVertices[j].x;
			v.Pos.y = mesh->mVertices[j].y;
			v.Pos.z = mesh->mVertices[j].z;

			XMFLOAT3 norm;
			norm.x = mesh->mNormals[j].x;
			norm.y = mesh->mNormals[j].y;
			norm.z = mesh->mNormals[j].z;

			v.Normal.x = mesh->mNormals[j].x;
			v.Normal.y = mesh->mNormals[j].y;
			v.Normal.z = mesh->mNormals[j].z;

			lineDirection.x = mesh->mNormals[j].x;
			lineDirection.y = mesh->mNormals[j].y;
			lineDirection.z = mesh->mNormals[j].z;

			lineDir.push_back(lineDirection);

			/*hair.m_pointType.Pos.x = mesh->mVertices[j].x;
			hair.m_pointType.Pos.y = mesh->mVertices[j].y;
			hair.m_pointType.Pos.z = mesh->mVertices[j].z;*/

			if (mesh->HasTextureCoords(0))
			{
				v.Tex.x = mesh->mTextureCoords[0][j].x;
				v.Tex.y = mesh->mTextureCoords[0][j].y;
			}
			positions.push_back(pos);
			normals.push_back(norm);

			m_vertices.push_back(v);
						
		}
		if (hasHair)
		{
			m_hairList = new LineClass(lineStarts, lineDir, pMesh->mNumVertices - 1, 2);
			m_hairList->Initialise(device, deviceContext);
		}

		for (int c = 0; c < mesh->mNumFaces; c++)
			for (int e = 0; e < mesh->mFaces[c].mNumIndices; e++)
				m_indices.push_back(mesh->mFaces[c].mIndices[e]);
	}

	/*LineClass* hair = new LineClass(positions, normals, m_numMeshes, 6);

	m_hairs.push_back(hair);*/

	m_assimpModel.m_numVertices = m_vertices.size();
	m_assimpModel.m_numFaces = m_indices.size() / 3;

	if (pScene->HasMaterials())
	{
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		{
			const aiMaterial* material = pScene->mMaterials[i];
			aiString texturePath;

			unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0
		}
	}

	for (int i = 0; i < pMesh->mNumFaces; i++)
	{
		if (pMesh->mFaces[i].mNumIndices == 3)
			m_numIndices += 3;
		else
		{
			MessageBox(NULL, "Failed to parse faces", "Error", MB_OK);
			return false;
		}
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Basic32) * m_vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_vertices[0];

	device->CreateBuffer(&vbd, &vinitData, &m_assimpModel.m_vertexBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * m_indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_indices[0];

	device->CreateBuffer(&ibd, &iinitData, &m_assimpModel.m_indexBuffer);

	result = LoadTexture(device, deviceContext, textureFileName);
	if (!result)
		return false;



	// Initialise the texture shader object.
	result = m_textureShader->Initialise(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the texture shader object.", "Error", MB_OK);
		return false;
	}
	for each (LineClass* strand in m_hairs)
	{
		strand->Initialise(device, deviceContext);
	}
	return true;
}

void AssimpImporter::Render(ID3D11DeviceContext* deviceContext)//ID3D11Device* device,, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	RenderBuffers(deviceContext);

	//m_textureShader->Render(deviceContext, GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, GetTexture());
	return;
	
}

void AssimpImporter::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT Stride = sizeof(Basic32);
	UINT Offset = 0;


	deviceContext->IASetVertexBuffers(0, 1, &m_assimpModel.m_vertexBuffer, &Stride, &Offset);
	deviceContext->IASetIndexBuffer(m_assimpModel.m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//deviceContext->DrawIndexed(m_assimpModel.m_numFaces * 3, 0, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

void AssimpImporter::RenderHairs(ID3D11Device* device, ID3D11DeviceContext* deviceContext)//, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	for (int i = 0; i < m_hairs.size(); i++)
	{
		m_hairs.at(i)->Render(deviceContext);
	}
}


void AssimpImporter::Shutdown()
{
	if (m_texture)
	{
		ReleaseTexture();
	}
	if (m_assimpModel.m_indexBuffer)
	{
		//ShutdownBuffers();
	}

	//ReleaseModel();
}

bool AssimpImporter::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
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

void AssimpImporter::ReleaseTexture()
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
