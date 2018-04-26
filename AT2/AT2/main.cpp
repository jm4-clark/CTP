#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	//Create the system object
	System = new SystemClass;
	if (!System)
		return 0;

	//Initialise & run system object
	result = System->Initialise();
	if (result)
		System->Run();

	//Shutdown & release the system object
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}