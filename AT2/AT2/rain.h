#ifndef _RAIN_H_
#define _RAIN_H_


#include <d3d11.h>
#include<DirectXMath.h>
#include <fstream>
#include "textureclass.h"

using namespace DirectX;

class Rain
{
public:
	Rain();
	Rain(const Rain&);
	~Rain();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Update(XMFLOAT3);

	ID3D11ShaderResourceView* GetTexture();

private:
	struct RainVertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 colour;
		XMFLOAT3 normal;
		XMFLOAT3 speed;
	};
	struct InstanceType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	bool InitialiseBuffers(ID3D11Device*);
	void UpdateBuffers(ID3D11DeviceContext*, float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_indexCount, m_instanceCount, m_maxInstanceCount;
	ModelType* m_model;
	TextureClass* m_texture;
};

#endif
