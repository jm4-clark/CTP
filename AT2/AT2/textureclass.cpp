#include "textureclass.h"

TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//load the targa image data into memory
	result = LoadTarga(filename, height, width);
	if (!result)
		return false;

	//set up the description of the texture
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
		return false;

	//set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	//copy the targa image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	//setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
		return false;

	//generate mipmaps for the texture
	deviceContext->GenerateMips(m_textureView);

	//release the targa image data now that the image data has been loaded into the texture
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	//release the texture view resource
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	//release the texture
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	//release the targa data
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//open the targa file for reading in binary
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	//read in the file header
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
		return false;

	//get the important information from the header
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;


	//check it's 32-bit, not 24
	if (bpp != 32)
		return false;

	//calculate the size of the 32-bit image data
	imageSize = width * height * 4;

	//allocate memory for the targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	//read in the targa image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
		return false;

	//close the file
	error = fclose(filePtr);
	if (error != 0)
		return false;

	//allocate memory for the targa destination data
	m_targaData = new unsigned char[imageSize];
	if (!targaImage)
		return false;

	//initialise the index into the targa destination data array
	index = 0;

	//initialise the into into the targa image data
	k = (width * height * 4) - (width * 4);

	//copy the targa image data into the targa destination array in the correct order
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  //Red
			m_targaData[index + 1] = targaImage[k + 1];  //Green
			m_targaData[index + 2] = targaImage[k + 0];  //Blue
			m_targaData[index + 3] = targaImage[k + 3];  //Alpha

			//increment the indexes into the targa data
			k += 4;
			index += 4;
		}

		//set the targa image data index back to the preceding row at the beginning of the column as its reading it in upside down
		k -= (width * 8);
	}

	//release the targa image data
	delete[] targaImage;
	targaImage = 0;

	return true;
}
