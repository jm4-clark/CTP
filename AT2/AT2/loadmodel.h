//#pragma once
//#include <includes.cpp>
//
//class Model
//{
//public:
//	Model();
//
//private:
//	HRESULT hr;
//
//	std::wifstream fileIn(filename.c_str());    //Open file
//	std::wstring meshMatLib;                    //String to hold our obj material library filename
//
//												//Arrays to store our model's information
//	std::vector<DWORD> indices;
//	std::vector<XMFLOAT3> vertPos;
//	std::vector<XMFLOAT3> vertNorm;
//	std::vector<XMFLOAT2> vertTexCoord;
//	std::vector<std::wstring> meshMaterials;
//
//	//Vertex definition indices
//	std::vector<int> vertPosIndex;
//	std::vector<int> vertNormIndex;
//	std::vector<int> vertTCIndex;
//
//	//Make sure we have a default if no tex coords or normals are defined
//	bool hasTexCoord = false;
//	bool hasNorm = false;
//
//	//Temp variables to store into vectors
//	std::wstring meshMaterialsTemp;
//	int vertPosIndexTemp;
//	int vertNormIndexTemp;
//	int vertTCIndexTemp;
//
//	wchar_t checkChar;        //The variable we will use to store one char from file at a time
//	std::wstring face;        //Holds the string containing our face vertices
//	int vIndex = 0;            //Keep track of our vertex index count
//	int triangleCount = 0;    //Total Triangles
//	int totalVerts = 0;
//	int meshTriangles = 0;
//};
//
