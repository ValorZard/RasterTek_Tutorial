#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

////////////////////////////////
// pre-processing directives //
//////////////////////////////
#define WIN32_LEAN_AND_MEAN // speeds up builds by getting rid of stuff we dont need

///////////////
// includes //
/////////////
#include <windows.h>

/////////////////////////
// our class includes //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"

///////////////////////////////
// Class name: System Class //
/////////////////////////////
class SystemClass
{
public:
	SystemClass(); // constructor
	SystemClass(const SystemClass&); // copy constructor (Object newObject = oldObject;)
	~SystemClass(); // destructor

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

//////////////////////////
// Function Prototypes //
////////////////////////
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // used to redirect windows system into MessageHandler()

//////////////
// Globals //
////////////

static SystemClass* ApplicationHandle = 0;

#endif