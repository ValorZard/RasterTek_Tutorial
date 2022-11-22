#include "systemclass.h"

SystemClass::SystemClass()
{
	// pretty sure this is the same as setting them to null
	// we set them to null ot make sure that theres no junk in there
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{
	// we dont do object clean up here, we'll do it in Shutdown() instead.
	// This is because some methods dont call the destructor for whatever reason
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// initialize width and height of screen to be zero
	screenWidth = 0;
	screenHeight = 0;

	// Initialize windows api
	InitializeWindows(screenWidth, screenHeight);

	// create and init the input object. Used to handle keyboard input
	m_Input = new InputClass;
	m_Input->Initialize();

	// Create and initialize the graphics object, which handles graphics
	m_Graphics = new GraphicsClass;

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);

	return result;
}


// cleans up all objects and pointers. Shutdown everything as well
void SystemClass::Shutdown()
{
	// release the graphics object
	if (m_Graphics) // "if m_Graphics exists and isn't null
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// release input object
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// shutdown window
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// loop until we get a quit message from window or user
	done = false;

	while (!done)
	{
		// handle windows messages first
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// if we get a signal to end app, we exit the app
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// else, we're just going to do frame processing
			result = Frame();
			if (!result) // user wants to quit for some reason
			{
				done = true;
			}
		}
	}
}

bool SystemClass::Frame()
{
	bool result;

	// Check if user pressed escape and wants to exit app
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for graphics object
	result = m_Graphics->Frame();
	
	if (!result) // for whatever reason, graphics wants to quit
	{
		return false;
	}

	return true;
}

// shove all the window system messages into here. We only care about certain info.
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// check if a key has been pressed on the keyboard
		case WM_KEYDOWN:
		{
			// if a key is pressed, send it to input object so it can record the state
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// check if a key has been released on the keyboard
		case WM_KEYUP:
		{
			// if a key is pressed, send it to input object so it can remove the state for that key
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// any other messages don't currently matter to us, so just do default stuff
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX windowClass; 
	DEVMODE dmScreenSettings;
	int posX, posY;

	// get external pointer to object
	ApplicationHandle = this;

	// get instance of the application
	m_hInstance = GetModuleHandle(NULL);

	// give the app a name
	m_applicationName = L"Engine"; // its a wchar_t (wide character array)
	
	// setup the windows class with default settings
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WndProc; // callback function
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = m_hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hIconSm = windowClass.hIcon;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = m_applicationName;
	windowClass.cbSize = sizeof(WNDCLASSEX);

	// register window class
	RegisterClassEx(&windowClass);

	// figure out size of client's desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN) // set in graphics class
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32; // displaying colors by allocating number of bits for color value
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// show mouse cursor
	ShowCursor(true);

	// fix display settings if we're in full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// remove app instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	// release pointer to this class
	ApplicationHandle = NULL;

	return;
}

// this is where windows send its messages to
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages go to the message handler in the system class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}