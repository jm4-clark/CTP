#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "textureclass.h"

using namespace DirectX;

class FontClass
{
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	FontType* m_font;
	TextureClass* m_texture;
};

#endif