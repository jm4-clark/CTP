#include "RainController.h"

RainController::RainController()
{
	m_instanceCount = 0;
	m_maxRainCount = 1000;
	m_rainPosTexture = 0;
	m_randTexture = 0;
}

RainController::RainController(const RainController& other)
{

}

RainController::~RainController()
{

}

RainController::RainController(XMFLOAT3 parentPos, int maxRainCount)
{
	/*m_instanceCount = 0;
	m_maxRainCount = maxRainCount;
	m_rainList.resize(m_maxRainCount);
	int maxX = parentPos.x + 100;
	int minX = parentPos.x - 100;
	int maxZ = parentPos.z + 100;
	int minZ = parentPos.z - 100;

	for (int i = 0; i < m_maxRainCount; i++)
	{
		int randX = rand() % (maxX - minX + 1) + minX;
		int randZ = rand() % (maxZ - minZ + 1) + minZ;
		m_rainList[i].pos = XMFLOAT3(randX, parentPos.y + 50, randZ);
		m_rainList[i].tex = XMFLOAT2(0, 0);
		m_rainList[i].active = false;
	}*/
}

bool RainController::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	bool result;
	result = InitialiseBuffers(device, deviceContext);
	if (!result)
		return false;

	result = InitialiseShaders(device, hwnd, L"../AT2/RainVertexShader.hlsl", L"..AT2/RainVertexShader.hlsl");
	if (!result)
		return false;

	return true;
}

void RainController::Shutdown()
{
	for (int i = 0; i < m_rainList.size(); i++)
	{
		//shutdown rain?
	}

	ShutdownBuffers();

	return;
}

bool RainController::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = SetShaderParameters(deviceContext);
	if (!result)
		return false;

	//RenderShader();

	RenderBuffers(deviceContext);


	return true;
}

bool RainController::InitialiseShaders(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "RainVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, "Rain Vertex Shader", "Missing Shader File", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "RainPixelShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, "Rain Pixel Shader", "Missing Shader File", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
		return false;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "SV_InstanceID";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 0;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
}

bool RainController::SetShaderParameters(ID3D11DeviceContext* deviceContext)//, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* dataPtr;
	unsigned int bufferNumber;

	result = deviceContext->Map(m_randTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr = (VertexType*)mappedResource.pData;

	deviceContext->Unmap(m_randTexture, 0);
}


bool RainController::InitialiseBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC tdesc,tdesc2;
	D3D11_SUBRESOURCE_DATA tbsd, tbsd2;

	int width = 128;
	int height = 128;
	int bpp = 12;
	int nb_color = 3;


	float *buf = new float[width * height * 3];

	int size = 128 * 128 * 3;
	for (int i = 0; i < size; i += 3)
	{
		float randX = rand() % 257;
		float randZ = rand() % 257;
		buf[i] = randX;
		buf[i + 1] = 100.0f;
		buf[i + 2] = randZ;
	}

	tbsd.pSysMem = (void*)buf;
	tbsd.SysMemPitch = width * bpp;
	tbsd.SysMemSlicePitch = width * height * bpp;

	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	//create the random texture
	result = device->CreateTexture2D(&tdesc, &tbsd, &m_randTexture);
	if (FAILED(result))
		return false;

	for (int i = 0; i < size; i += 3)
	{
		buf[i] = 1.0f;
		buf[i + 1] = 1.0f;
		buf[i + 2] = 1.0f;
	}

	tbsd2.pSysMem = (void*)buf;
	tbsd2.SysMemPitch = width * bpp;
	tbsd2.SysMemSlicePitch = width * height * bpp;

	tdesc2.Width = width;
	tdesc2.Height = height;
	tdesc2.MipLevels = 1;
	tdesc2.ArraySize = 1;
	tdesc2.SampleDesc.Count = 1;
	tdesc2.SampleDesc.Quality = 0;
	tdesc2.Usage = D3D11_USAGE_DEFAULT;
	tdesc2.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tdesc2.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc2.CPUAccessFlags = 0;
	tdesc2.MiscFlags = 0;

	//create the empty texture
	result = device->CreateTexture2D(&tdesc2, &tbsd2, &m_rainPosTexture);
	if (FAILED(result))
		return false;

	delete[] buf;
	
	/*
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//
	//BufferType* dataPtr;

	//int rainCount = sizeof(m_rainList) / sizeof(m_rainList[0]);
	//unsigned long* indices;

	//indices = new unsigned long[m_maxRainCount];
	//if (!indices)
	//	return false;

	//m_randomColors.resize(128 * 128);
	//for (int i = 0; i < m_randomColors.size(); i++)
	//{
	//	int randX = rand() % 257;
	//	int randZ = rand() % 257;
	//	m_randomColors[i].x = randX;
	//	m_randomColors[i].y = 100;
	//	m_randomColors[i].z = randZ;
	//	m_randomColors[i].w = 256;
	//}


	//m_randTexDesc.Width = 128;
	//m_randTexDesc.Height = 128;
	//m_randTexDesc.MipLevels = m_randTexDesc.ArraySize = 1;
	//m_randTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//m_randTexDesc.SampleDesc.Count = 1;
	//m_randTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_randTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//m_randTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//m_randTexDesc.MiscFlags = 0;
	//
	//D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	//

	//D3D11_SUBRESOURCE_DATA randImageData;
	//ZeroMemory(&randImageData, sizeof(randImageData));
	//randImageData.pSysMem = &m_randomColors[0];
	//randImageData.SysMemPitch = 0;
	//randImageData.SysMemSlicePitch = 0;
	//

	//device->CreateTexture2D(&m_randTexDesc, &randImageData, &m_randTexture);

	//result = deviceContext->Map(m_randTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//	return false;

	//dataPtr = (BufferType*)mappedResource.pData;

	//dataPtr->colours.resize(128 * 128);

	//

	//for (int i = 0; i < 128 * 128; i++)
	//{
	//	int randX = rand() % (256 - 0 + 1) + 0;
	//	int randZ = rand() % (256 - 0 + 1) + 0;
	//	dataPtr->colours[i].x = 
	//}

	//deviceContext->Unmap(m_randTexture, 0);
	
	////device->CreateTexture2D(&m_randTexDesc, NULL, &m_randTexture); //NULL will be initial data, e.g. random positions for rain ???
	
	//m_rainPosTexDesc.Width = 128;
	//m_rainPosTexDesc.Height = 128;
	//m_rainPosTexDesc.MipLevels = m_randTexDesc.ArraySize = 1;
	//m_rainPosTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//m_rainPosTexDesc.SampleDesc.Count = 1;
	//m_rainPosTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_rainPosTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//m_rainPosTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	//m_rainPosTexDesc.MiscFlags = 0;

	//result = deviceContext->Map(m_rainPosTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//	return false;


	//deviceContext->Unmap(m_rainPosTexture, 0);

	//device->CreateTexture2D(&m_rainPosTexDesc, NULL, &m_rainPosTexture); //NULL will be initial data, e.g. random positions for rain ???*/

	return true;

}

void RainController::ShutdownBuffers()
{
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

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void RainController::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//m_instanceBuffer->Release();
	//m_instanceBuffer = nullptr;
	//ID3D11Device* temp;
	//deviceContext->GetDevice(&temp);
	//temp->CreateBuffer(&m_instanceDesc, &m_instanceData, &m_instanceBuffer);
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(RainType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 1;

	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	//// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawInstanced(6, 128*128, 0, 0);

	return;
}

void RainController::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "rain shader", MB_OK);

	return;
}


bool RainController::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource, mappedResource2;
	VertexType* vertices;
	VertexType* verticesPtr;
	RainType *rainPtr1, *rainPtr2;
	HRESULT result;

	ID3D11Texture2D *texPtr1, *texPtr2;

	result = deviceContext->Map(m_rainPosTexture, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
	if (FAILED(result))
		return false;

	result = deviceContext->Map(m_randTexture, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource2);
	if (FAILED(result))
		return false;

	texPtr1 = (ID3D11Texture2D*)mappedResource.pData;
	texPtr2 = (ID3D11Texture2D*)mappedResource2.pData;





	m_switch = !m_switch;



	float *buf, *temp1, *temp2 = new float[128 * 128 * 3];

	vertices = new VertexType[128 * 128];

	

	deviceContext->Unmap(m_rainPosTexture, 0);

	deviceContext->Unmap(m_randTexture, 0);

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;
	
	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * 128 * 128));
	
	// Unmap the texture & clean up
	deviceContext->Unmap(m_randTexture, 0);
}

void RainController::RenderShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//deviceContext->DrawInstanced();
}

//void RainController::RenderInstanced()
//{
//
//}

//void RainController::SetRenderTarget(ID3D11DeviceContext* deviceContext)
//{
//	deviceContext->OMSetRenderTargets(1, &m_)
//}

