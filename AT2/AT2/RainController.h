#ifndef _RAINCONTROLLER_H_
#define _RAINCONTROLLER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>
#include <fstream>

using namespace DirectX;


class RainController
{
public:
	RainController();
	RainController(const RainController&);
	RainController(XMFLOAT3, int);
	~RainController();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);
	void Update(ID3D11Device*);
	
	ID3D11Texture2D* GetTexture1() { return m_randTexture; };
	ID3D11Texture2D* GetTexture2() { return m_rainPosTexture; };
private:
	struct RainType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 norm;
		//bool active;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		float TotalTime;
		float Life;
		float Distance;
	};

	struct InstanceType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT3 norm;
		bool active;
	};

	struct BufferType
	{
		//std::vector<XMFLOAT4> colours;

	};

	bool InitialiseShaders(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*);//, ID3D11ShaderResourceView*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool InitialiseBuffers(ID3D11Device*, ID3D11DeviceContext*);
	bool UpdateBuffers(ID3D11DeviceContext*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void RenderShader(ID3D11DeviceContext*);

	int m_maxRainCount;
	int m_instanceCount;
	std::vector<RainType> m_rainList;
	std::vector<InstanceType> instanceDataVector;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	ID3D11Buffer* m_instanceBuffer;
	D3D11_SUBRESOURCE_DATA m_instanceData;
	D3D11_BUFFER_DESC m_instanceDesc;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;

	D3D11_TEXTURE2D_DESC m_randTexDesc, m_rainPosTexDesc;
	ID3D11Texture2D *m_randTexture, *m_rainPosTexture;

	std::vector<XMFLOAT4> m_randomColors;

	bool m_switch;
};

#endif