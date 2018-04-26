#include "colourshaderclass.h"

ColourShaderClass::ColourShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColourShaderClass::ColourShaderClass(const ColourShaderClass& other)
{
}


ColourShaderClass::~ColourShaderClass()
{
}

bool ColourShaderClass::Initialise(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitialiseShader(device, hwnd, L"../AT2/color.vs", L"../AT2/color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColourShaderClass::Shutdown()
{
	//shutdown vertex & pixel shaders
	ShutdownShader();

	return;
}

bool ColourShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColourShaderClass::InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile vertex shader code
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColourVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//if shader failed to compile, there should be an error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//but if there wasn't, then the shader itself couldn't be found
		else
		{
			MessageBox(hwnd, "", "Missing Shader File", MB_OK); //vsFilename - WCHAR* incompatible with LPCSTR
		}
		return false;
	}
	
	//compile pixel shader code
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColourPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, " ", "Missing Shader File", MB_OK); //psFilename - WCHAR* incompatible with LPCSTR
		}

		return false;
	}

	//create vertex shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
		return false;

	//create pixel shader
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
		return false;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//get count of elements in layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;

	//release the vertex & pixel shaders as they aren't needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
		return false;

	return true;
 }

 void ColourShaderClass::ShutdownShader()
 {
	 // Release the matrix constant buffer.
	 if (m_matrixBuffer)
	 {
		 m_matrixBuffer->Release();
		 m_matrixBuffer = 0;
	 }

	 // Release the layout.
	 if (m_layout)
	 {
		 m_layout->Release();
		 m_layout = 0;
	 }

	 // Release the pixel shader.
	 if (m_pixelShader)
	 {
		 m_pixelShader->Release();
		 m_pixelShader = 0;
	 }

	 // Release the vertex shader.
	 if (m_vertexShader)
	 {
		 m_vertexShader->Release();
		 m_vertexShader = 0;
	 }

	 return;
 }

 void ColourShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
 {
	 char* compileErrors;
	 unsigned long long bufferSize, i;
	 std::ofstream fout;

	 //get a pointer to the error message text buffer
	 compileErrors = (char*)(errorMessage->GetBufferPointer());

	 //get length of message.
	 bufferSize = errorMessage->GetBufferSize();

	 //open a file to write the error message to.
	 fout.open("shader-error.txt");

	 //write out the error message.
	 for (i = 0; i<bufferSize; i++)
	 {
		 fout << compileErrors[i];
	 }

	 //close file
	 fout.close();

	 //Release the error message
	 errorMessage->Release();
	 errorMessage = 0;

	 //pop up message on screen to check text file for errors
	 MessageBox(hwnd, "Error compiling shader. Check shader-error.txt for details.", "_", MB_OK); //shaderFilename

	 return;
 }

 bool ColourShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	 XMMATRIX projectionMatrix)
 {
	 HRESULT result;
	 D3D11_MAPPED_SUBRESOURCE mappedResource;
	 MatrixBufferType* dataPtr;
	 unsigned int bufferNumber;

	 //Transpose matrices to prepare them for shader
	 worldMatrix = XMMatrixTranspose(worldMatrix);
	 viewMatrix = XMMatrixTranspose(viewMatrix);
	 projectionMatrix = XMMatrixTranspose(projectionMatrix);

	 //lock constant buffer so it can be written to
	 result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	 if (FAILED(result))
		 return false;

	 //get pointer to data in constant buffer
	 dataPtr = (MatrixBufferType*)mappedResource.pData;

	 //copy the matrices into the constant buffer
	 dataPtr->world = worldMatrix;
	 dataPtr->view = viewMatrix;
	 dataPtr->proj = projectionMatrix;

	 // Unlock the constant buffer.
	 deviceContext->Unmap(m_matrixBuffer, 0);

	 // Set the position of the constant buffer in the vertex shader.
	 bufferNumber = 0;

	 // Finanly set the constant buffer in the vertex shader with the updated values.
	 deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	 return true;
 }

 void ColourShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
 {
	 // Set the vertex input layout.
	 deviceContext->IASetInputLayout(m_layout);

	 // Set the vertex and pixel shaders that will be used to render this triangle.
	 deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	 deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	 // Render the triangle.
	 deviceContext->DrawIndexed(indexCount, 0, 0);

	 return;
 }
