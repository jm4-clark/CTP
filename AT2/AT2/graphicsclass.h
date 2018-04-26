#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <AntTweakBar.h>
#include "assimpImporter.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colourshaderclass.h"
#include "textureshaderclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "bitmapclass.h"
#include "LineShader.h"
#include "textclass.h"
#include "RenderTextureClass.h"
#include "DebugWindowClass.h"
#include "ShadowShaderClass.h"
#include "DepthShaderClass.h"
#include "rain.h"
#include "RainController.h"

#include <cmath>
#include <vector>
#include <cassert>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, XMFLOAT3, XMFLOAT3);

	XMFLOAT3 GetCamPos() { return m_cam->GetPosition(); };
	XMFLOAT3 GetCamRot() { return m_cam->GetRotation(); };

private:
	bool Render();
	bool RenderHair();

	bool RenderToTexture();
	bool RenderScene();

	XMMATRIX QuaternionToMatrix(const aiQuaternion& quat);
	TwSetVarCallback GraphicsClass::ResetCamPos();

	Rain* m_rain;

	D3DClass* m_Direct3D;
	CameraClass* m_cam;
	ModelClass* m_model;
	ColourShaderClass* m_colourShader;
	TextureShaderClass* m_modelTextureShader;
	TextureShaderClass* m_assimpTextureShader;
	TextureShaderClass* m_textureShader;
	//LineShader* m_lineShader;
	LightClass* m_light;

	RenderTextureClass* m_debugRenderTexture;
	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;
	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_lightShader;
	BitmapClass* m_bitmap;
	AssimpImporter *m_assimpModel, *m_assimpScalp;
	
	ModelClass *m_floor, *m_billboard, *m_cube, *m_sphere;

	
	aiQuaternion m_modelRotation;

	int m_screenWidth, m_screenHeight;

	TextClass* m_text;
	ShadowShaderClass* m_shadowShader;
	DepthShaderClass* m_depthShader;

	RainController* m_rainController;

};

#endif