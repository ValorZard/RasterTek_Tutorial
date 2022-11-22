#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// create the system object
	System = new SystemClass;

	// initialize and run system object
	result = System->Initialize();

	if (result == true)
	{
		System->Run();
	}

	// after system is running, we are now able to shut it down
	System->Shutdown();
	delete System; // c++ remember, need to make sure all of our pointers dont have memory attached, else we get leaks
	System = 0; // I think you can also set this to null, not sure tho

	return 0;
}