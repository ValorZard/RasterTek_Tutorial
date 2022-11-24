#include "systemclass.h"

/// <summary>
/// I'm currently on https://www.rastertek.com/dx11s3tut02.html
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="pScmdline"></param>
/// <param name="iCmdshow"></param>
/// <returns></returns>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// create the system object
	System = new SystemClass;

	// initialize and run system object
	result = System->Initialize();

	if (result) // if result is valid
	{
		System->Run();
	}

	// after system is running, we are now able to shut it down
	System->Shutdown();
	delete System; // c++ remember, need to make sure all of our pointers dont have memory attached, else we get leaks
	System = 0; // I think you can also set this to null, not sure tho

	return 0;
}