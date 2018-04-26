#ifndef _ASSIMP_IMPORTER_H_
#define _ASSIMP_IMPORTER_H_

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "Include\assimp\Importer.hpp"
#include "Include\assimp\postprocess.h"
#include "Include\assimp\scene.h"
#include "textureclass.h"
#include "textureshaderclass.h"
#include "LineClass.h"


using namespace Assimp;
using namespace DirectX;

class AssimpImporter
{
public:
	AssimpImporter();
	~AssimpImporter();

	bool LoadModel(ID3D11Device*, ID3D11DeviceContext*, HWND hwnd, const char*, char*, bool);
	void Render(ID3D11DeviceContext*);
	void RenderHairs(ID3D11Device*, ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

	void Shutdown();

	int GetIndexCount() { return m_numIndices; };
	ID3D11ShaderResourceView* GetTexture() { return m_texture->GetTexture(); };

	struct ModelData
	{
		ModelData()
		{
			m_vertexBuffer = nullptr;
			m_indexBuffer = nullptr;
		}
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		UINT m_numVertices;
		UINT m_numFaces;
	};

	struct Basic32
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};
	
	struct NormalMap
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT3 Tangent;
	};
	std::vector<LineClass*> m_hairs;
	ModelData m_assimpModel;
	LineClass* m_hairList;
private:

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	Importer imp;
	const aiScene* pScene;
	const aiMesh* pMesh;

	/*ModelData m_model;*/
	Basic32 m_basic32;
	NormalMap m_normalMap;

	UINT m_numIndices;
	std::vector<Basic32> m_vertices;
	std::vector<DWORD> m_indices;
	int m_numMeshes;
	
	
	
	TextureClass* m_texture;
	TextureShaderClass* m_textureShader;
};

#endif

