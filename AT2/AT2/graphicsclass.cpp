#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_cam = 0;
	m_model = 0;
	m_light = 0;
	m_lightShader = 0;
	m_bitmap = 0;
	m_textureShader = 0;
	m_assimpTextureShader = 0;
	m_cube = 0;
	m_RenderTexture = 0;
	m_DebugWindow = 0;
	m_TextureShader = 0;
	m_sphere = 0;
	m_floor = 0;
	m_billboard = 0;

	m_screenWidth = 0;
	m_screenHeight = 0;

	m_text = 0;

	m_depthShader = 0;

	m_rain = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialise(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialise(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_cam = new CameraClass;
	if (!m_cam)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_cam->SetPosition(0.0f, 0.0f, -50.0f);
	m_cam->Render();
	m_cam->GetViewMatrix(baseViewMatrix);

	m_rain = new Rain;
	if (!m_rain)
		return false;

	result = m_rain->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/square.txt", "../AT2/Resources/grass.tga");
	if (!result)
	{
		return false;
	}

	m_assimpModel = new AssimpImporter;
	if (!m_assimpModel)
	{
		MessageBox(hwnd, "Could not initialise rain model object.", "Error", MB_OK);
		return false;
	}



	result = m_assimpModel->LoadModel(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, "../AT2/Resources/Free_Cartoon_Land_2.obj", "../AT2/Resources/grass.tga", false);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise assimp model object.", "Error", MB_OK);
		return false;
	}

	/*m_assimpScalp = new AssimpImporter;
	if (!m_assimpScalp)
	{
		return false;
	}

	result = m_assimpScalp->LoadModel(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, "../AT2/Resources/male_scalp.obj", "../AT2/Resources/grass.tga", true);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise assimp scalp model object.", "Error", MB_OK);
	}*/

	// Create the model object.
	m_model = new ModelClass;
	if (!m_model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_model->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/floor.txt", "../AT2/Resources/o_o.tga");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the model object.", "Error", MB_OK);
		return false;
	}

	m_cube = new ModelClass;
	if (!m_cube)
		return false;

	result = m_cube->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/cube.txt", "../AT2/Resources/grass.tga");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise cube", "Error", MB_OK);
		return false;
	}
	m_cube->SetPosition(-2.0f, 2.0f, 0.0f);

	m_sphere = new ModelClass;
	if (!m_sphere)
		return false;

	result = m_sphere->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/sphere.txt", "../AT2/Resources/grass.tga");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise sphere", "Error", MB_OK);
		return false;
	}
	m_sphere->SetPosition(2.0f, 2.0f, 0.0f);



	// Create the texture shader object.
	m_assimpTextureShader = new TextureShaderClass;
	if (!m_assimpTextureShader)
		return false;

	// Initialise the color shader object.
	result = m_assimpTextureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the color shader object.", "Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_lightShader = new LightShaderClass;
	if (!m_lightShader)
		return false;


	// Initialize the light shader object.
	result = m_lightShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the light shader object.", "Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_light = new LightClass;
	if (!m_light)
		return false;

	// Initialise the light object.
	m_light->SetAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColour(0.0f, 1.0f, 0.0f, 1.0f);
	m_light->SetDirection(1.0f, 0.0f, 0.0f);
	m_light->SetLookAt(0.0f, 0.0f, 0.0f);
	m_light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	//create the tender to texture object
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
		return false;

	//result = m_RenderTexture->Initialise(m_Direct3D->GetDevice(), screenWidth, screenHeight); 
	result = m_RenderTexture->Initialise(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
		return false;

	m_depthShader = new DepthShaderClass;
	if (!m_depthShader)
		return false;

	result = m_depthShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the depth shader object.", "Error", MB_OK);
		return false;
	}

	// Create the shadow shader object.
	m_shadowShader = new ShadowShaderClass;
	if (!m_shadowShader)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_shadowShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the shadow shader object.", "Error", MB_OK);
		return false;
	}

	//create the tender to texture object
	m_debugRenderTexture = new RenderTextureClass;
	if (!m_debugRenderTexture)
		return false;

	result = m_debugRenderTexture->Initialise(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
		return false;

	//Create the debug window object.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
		return false;

	result = m_DebugWindow->Initialise(m_Direct3D->GetDevice(), screenWidth, screenHeight, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the debug window object", "Error", MB_OK);
		return false;
	}


	// Create the texture shader object.
	m_textureShader = new TextureShaderClass;
	if (!m_textureShader)
		return false;

	// Initialise the color shader object.
	result = m_textureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	// Initialise the color shader object.
	result = m_TextureShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the texture shader object.", "Error", MB_OK);
		return false;
	}


	m_text = new TextClass;
	if (!m_text)
	{
		return false;
	}

	result = m_text->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, m_screenWidth, m_screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	m_depthShader = new DepthShaderClass;
	if (!m_depthShader)
		return false;

	result = m_depthShader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise the depth shader object", "Error", MB_OK);
		return false;
	}

	m_rainController = new RainController;
	if (!m_rainController)
		return false;

	//////////////unfortunately, due to errors within the incomplete vertex shader, the rainController cannot be initialised without throwing an error
	/*result = m_rainController->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise rain controller object", "Error", MB_OK);
		return false;
	}*/

	//create bitmap object
	m_bitmap = new BitmapClass;
	if (!m_bitmap)
		return false;
	
	//initialise bitmap object
	result = m_bitmap->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, "../AT2/Resources/size32/tga/env_light_database/cv0_osc0.tga", 32, 1050);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise bitmap object", "Error", MB_OK);
		return false;
	}

	m_floor = new ModelClass;
	if (!m_floor)
		return false;

	result = m_floor->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/floor.txt", "../AT2/Resources/doh.tga");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise floor object", "Error", MB_OK);
		return false;
	}

	m_billboard = new ModelClass;
	if (!m_billboard)
		return false;

	result = m_billboard->Initialise(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../AT2/Resources/square.txt", "../AT2/Resources/cheers.tga");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialise billboard object", "Error", MB_OK);
		return false;
	}

	//AntTweakBar initialisation
	if (!TwInit(TW_DIRECT3D11, m_Direct3D->GetDevice()))
	{
		MessageBox(hwnd, TwGetLastError(), "AntTweakBar Initialisation failed", MB_OK);
		return false;
	}

	//m_modelRotation.x = .0f;

	TwWindowSize(800,600);
	TwBar *bar;
	float camX = 0;
	bar = TwNewBar("Ant Tweak Bar");
	TwDefine(" GLOBAL help='This is an Ant Tweak Bar' ");
	int barSize[2] = { 224, 320 };
	TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);
	//bar variables
	TwAddVarRW(bar, "Scene Rotation", TW_TYPE_QUAT4F, &m_modelRotation, "opened=true axis=-z group=Model");

	

	/*XMFLOAT3 axis = XMFLOAT3(-1, 1, 0);
	m_modelRotation = RotationFromAxisAngle(axis, 3.1415 / 4);*/


	return true;
}


void GraphicsClass::Shutdown()
{
	TwTerminate();
	if (m_rain)
	{
		m_rain->Shutdown();
		delete m_rain;
		m_rain = 0;
	}
	if (m_shadowShader)
	{
		m_shadowShader->Shutdown();
		delete m_shadowShader;
		m_shadowShader = 0;
	}
	if (m_sphere)
	{
		m_sphere->Shutdown();
		delete m_sphere;
		m_sphere = 0;
	}
	if (m_cube)
	{
		m_cube->Shutdown();
		delete m_cube;
		m_cube = 0;
	}
	if (m_depthShader)
	{
		m_depthShader->Shutdown();
		delete m_depthShader;
		m_depthShader = 0;
	}
	// Release the text object.
	if (m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = 0;
	}
	// Release the debug window object.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}
	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	// Release the render to texture object.
	if (m_debugRenderTexture)
	{
		m_debugRenderTexture->Shutdown();
		delete m_debugRenderTexture;
		m_debugRenderTexture = 0;
	}
	if (m_floor)
	{
		m_floor->Shutdown();
		delete m_floor;
		m_floor = 0;
	}
	if (m_billboard)
	{
		m_billboard->Shutdown();
		delete m_billboard;
		m_billboard = 0;
	}
	if (m_assimpModel)
	{
		m_assimpModel->Shutdown();
		delete m_assimpModel;
		m_assimpModel = 0;
	}
	if (m_assimpScalp)
	{
		m_assimpScalp->Shutdown();
		delete m_assimpScalp;
		m_assimpScalp = 0;
	}
	// Release the bitmap object.
	if (m_bitmap)
	{
		m_bitmap->Shutdown();
		delete m_bitmap;
		m_bitmap = 0;
	}
	// Release the light object.
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// Release the light shader object.
	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		delete m_lightShader;
		m_lightShader = 0;
	}
	
	// Release the texture shader object.
	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the texture shader object.
	if (m_assimpTextureShader)
	{
		m_assimpTextureShader->Shutdown();
		delete m_assimpTextureShader;
		m_assimpTextureShader = 0;
	}

	// Release the model object.
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	// Release the camera object.
	if (m_cam)
	{
		delete m_cam;
		m_cam = 0;
	}
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, float frameTime, XMFLOAT3 camPos, XMFLOAT3 camRot)
{
	bool result;
	static float lightPosX = -5.0f;
	//result = m_text->SetFps(fps, m_Direct3D->GetDeviceContext());
	m_cam->SetPosition(camPos.x, camPos.y, camPos.z);
	m_cam->SetRotation(camRot.x, camRot.y, camRot.z);

	lightPosX += 0.05f;
	if (lightPosX > 5.0f)
	{
		lightPosX = -5.0f;
	}
	m_light->SetPosition(lightPosX, 8.0f, -5.0f);

	result = m_text->SetFps(fps, m_Direct3D->GetDeviceContext());
	if (!result)
		return false;

	result = m_text->SetCpu(cpu, m_Direct3D->GetDeviceContext());
	if (!result)
		return false;

	
	
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	bool result;

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;

	//render the entire scene to the texture first
	result = RenderToTexture();
	if (!result)
		return false;

	XMFLOAT3 camPos, modelPos;
	double angle;
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.2f, 0.0f, 0.0f, 1.0f);

	

	result = RenderScene();
	if (!result)
		return false;

	//turn off z buffer to begin 2d rendering
	m_Direct3D->TurnZBufferOff();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_cam->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	result = m_bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100);
	if (!result)
		return false;
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_bitmap->GetTexture());
	if (!result)
		return false;

	result = m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 0, 0); //m_screenWidth -125, 25);
	if (!result)
		return false;

	result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_debugRenderTexture->GetShaderResourceView());
	if (!result)
		return false;

	m_Direct3D->TurnOnAlphaBlending();
	result = m_text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;
	m_Direct3D->TurnOffAlphaBlending();


	//turn z buffer back on
	m_Direct3D->TurnZBufferOn();
	//m_rain->Render(m_Direct3D->GetDeviceContext());
	//result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), 6, worldMatrix, viewMatrix, projectionMatrix, m_rain->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	/*
	///////////////////////2D stuff
	//turn off z-buffer to begin 2d rendering
	//m_Direct3D->TurnZBufferOff();
	//
	//put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing
	//result = m_bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100);
	//if (!result)
		//return false;
//
	//turn z buffer back on now that 2d rendering is done
	//m_Direct3D->TurnZBufferOn();
	///////////////////////2D stuff

	//rotate the world matrix by the rotation value so the triangle will spin
	//XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMMATRIX rot = XMMatrixRotationY(m_modelRotation.y);
	//result = m_assimpTextureShader->Render(m_Direct3D->GetDeviceContext(), m_floor->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_floor->GetTexture());
	
	/*result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), m_assimpModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_assimpModel->GetTexture(), m_light->GetDirection(), m_light->GetAmbientColour(), m_light->GetDiffuseColour());
	if (!result)
		return false;

	//m_assimpScalp->Render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//result = m_assimpTextureShader->Render(m_Direct3D->GetDeviceContext(), m_assimpScalp->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_assimpScalp->GetTexture());
	//if (!result)
	//	return false;
	//m_assimpScalp->Render(m_Direct3D->GetDeviceContext());//, m_assimpScalp->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_assimpScalp->GetTexture())
	
	

	//result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_floor->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_floor->GetTexture());
	//if (!result)
	//	return false;

	////BILLBOARD STUFF
	//camPos = m_cam->GetPosition();
	//modelPos.x = 0.0f;
	//modelPos.y = 1.5f;
	//modelPos.z = 0.0f;

	//angle = atan2(modelPos.x - camPos.x, modelPos.z - camPos.z) * (180.0 / 3.1415);

	//rotation = (float)angle * 0.0174532925f;
	//worldMatrix = XMMatrixRotationY(rotation);

	//translateMatrix = XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);

	//worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	/*m_billboard->Render(m_Direct3D->GetDeviceContext());
	result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_billboard->GetVertexCount(), m_billboard->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, m_billboard->GetTexture());
	if (!result)
	{
		return false;
	}
	//END OF BILLBOARD STUFF
	
	
	

	// Render the model using the light shader.
	//result = m_cubeTextureShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture());
	//result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), m_assimpModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,	
		//m_assimpModel->GetTexture(), m_light->GetDirection(), m_light->GetAmbientColour(), m_light->GetDiffuseColour());
	//if (!result)
	//	return false;
	*/

	TwDraw();
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	XMFLOAT3 pos;

	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	m_debugRenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	//set background to blue to differentiate it
	m_debugRenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0, 0.0, 1.0f, 1.0f);

	m_light->GenerateViewMatrix();
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_light->GetViewMatrix(lightViewMatrix);
	m_light->GetProjectionMatrix(lightProjectionMatrix);

	//m_cube->GetPosition(pos.x, pos.y, pos.z);
	//worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	/*m_cube->Render(m_Direct3D->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_cube->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
		return false;

	m_Direct3D->GetWorldMatrix(worldMatrix);*/

	//m_sphere->GetPosition(pos.x, pos.y, pos.z);
	//worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	/*m_sphere->Render(m_Direct3D->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_sphere->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
		return false;

	m_Direct3D->GetWorldMatrix(worldMatrix);*/

	//m_floor->GetPosition(pos.x, pos.y, pos.z);
	//worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	/*m_floor->Render(m_Direct3D->GetDeviceContext());
	result = m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_floor->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
		return false;*/

	result = RenderScene();
	if (!result)
		return false;

	m_Direct3D->SetBackBufferRenderTarget();

	//m_Direct3D->ResetViewport();

	return true;
}

bool GraphicsClass::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX lightViewMatrix, lightProjMatrix;
	bool result;
	XMFLOAT3 pos;

	// Generate the view matrix based on the camera's position.
	m_cam->Render();

	//m_light->GenerateViewMatrix();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_cam->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	
	//m_light->GetViewMatrix(lightViewMatrix);
	//m_light->GetProjectionMatrix(lightProjMatrix);


	worldMatrix = QuaternionToMatrix(m_modelRotation);


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	/*m_cube->GetPosition(pos.x, pos.y, pos.z);
	worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);*/
	
	//m_cube->Render(m_Direct3D->GetDeviceContext());
	// Render the model using the shadow shader.
	//result = m_shadowShader->Render(m_Direct3D->GetDeviceContext(), m_cube->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjMatrix, m_cube->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_light->GetPosition(),
	//	m_light->GetAmbientColour(), m_light->GetDiffuseColour());
	//if (!result)
	//{
	//	return false;
	//}

	//m_Direct3D->GetWorldMatrix(worldMatrix);

	//m_sphere->GetPosition(pos.x, pos.y, pos.z);
	//worldMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	//m_sphere->Render(m_Direct3D->GetDeviceContext());
	// Render the model using the shadow shader.
	//result = m_shadowShader->Render(m_Direct3D->GetDeviceContext(), m_sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjMatrix, m_sphere->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_light->GetPosition(),
	//	m_light->GetAmbientColour(), m_light->GetDiffuseColour());
	//if (!result)
	//{
	//	return false;
	//}

	//m_Direct3D->GetWorldMatrix(worldMatrix);

	//m_model->Render(m_Direct3D->GetDeviceContext());
	//result = m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture());
	//result = m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//if (!result)
	//	return false;

	
	m_assimpModel->Render(m_Direct3D->GetDeviceContext());
	//result = m_lightShader->Render(m_Direct3D->GetDeviceContext(), m_assimpModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_assimpModel->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColour());
	result = m_assimpTextureShader->Render(m_Direct3D->GetDeviceContext(), m_assimpModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_assimpModel->GetTexture());
	if (!result)
		return false;
	

	

	//result = m_assimpTextureShader->Render(m_Direct3D->GetDeviceContext(), m_assimpScalp->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_assimpScalp->GetTexture());
	//if (!result)
	//	return false;

	//m_assimpScalp->m_hairList->Render(m_Direct3D->GetDeviceContext());


	//m_Direct3D->GetWorldMatrix(worldMatrix);

	//m_floor->Render(m_Direct3D->GetDeviceContext());


	return true;
}

bool GraphicsClass::RenderHair()
{
	return true;
}

XMMATRIX GraphicsClass::QuaternionToMatrix(const aiQuaternion& quat)
{
	//XMMATRIX out;
	float yy2 = 2.0f * quat.y * quat.y;
	float xy2 = 2.0f * quat.x * quat.y;
	float xz2 = 2.0f * quat.x * quat.z;
	float yz2 = 2.0f * quat.y * quat.z;
	float zz2 = 2.0f * quat.z * quat.z;
	float wz2 = 2.0f * quat.w * quat.z;
	float wy2 = 2.0f * quat.w * quat.y;
	float wx2 = 2.0f * quat.w * quat.x;
	float xx2 = 2.0f * quat.x * quat.x;
	XMMATRIX out = XMMATRIX(-yy2 - zz2 + 1.0f, xy2 + wz2, xz2 - wy2, 0, 
		xy2 - wz2, -xx2 - zz2 + 1.0f, yz2 + wx2,0, 
		xz2 + wy2, yz2 - wx2, -xx2 - yy2 + 1.0f, 0,
		0, 0 , 0, 1);
	return out;
}