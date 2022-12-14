#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////
// includes //
/////////////
#include <windows.h>

//////////////
// globals //
////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////
// class name: GraphicsClass //
//////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:

};
#endif
