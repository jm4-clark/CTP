#ifndef _LINE_CLASS_H_
#define _LINE_CLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class LineClass
{
public:
	LineClass();
	LineClass(std::vector<XMFLOAT3>, std::vector<XMFLOAT3>, int, int);
	LineClass(XMFLOAT3, XMFLOAT3);//std::vector<XMFLOAT3>, std::vector<XMFLOAT3>, int);
	LineClass(const LineClass&);
	~LineClass();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	//void Update(ID3D11DeviceContext*, float);
	int GetIndexCount() { return m_indexCountPerHair; };
	void TogglePhysics() { m_physicsOn = !m_physicsOn; };

	struct ModelType
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Norm;
	};

	struct Basic32
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct Basic32Line
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
	};
	std::vector<Basic32Line> m_lineInfo;

	struct PointType
	{
		XMFLOAT3 Pos;
		XMFLOAT3 norm;
	};

	struct InstanceType
	{
		XMFLOAT3 Pos;
	};
	//PointType m_pointType;
private:
	
	
	bool InitialiseBuffers(ID3D11Device*);
	void UpdateBuffers(ID3D11DeviceContext*, float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool m_physicsOn;
	Basic32 parent;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	D3D11_SUBRESOURCE_DATA instanceData;
	D3D11_BUFFER_DESC instanceDesc;
	std::vector<InstanceType> instanceDataVector;

	int m_instanceCount;
	int m_vertexCountPerHair, m_indexCountPerHair, m_parentCount;
	std::vector<Basic32> m_parentList;
	int m_lengthOfHair, m_numOfHairs;

	std::vector<PointType> m_strands;
	std::vector<Basic32> m_vertex;
	std::vector<PointType> vertices;
};

#endif