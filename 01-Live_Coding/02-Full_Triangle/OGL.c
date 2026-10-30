// header Files
#include <windows.h>
#include <stdio.h>
#include "OGL.h"
#include <stdlib.h>

//OpenGL header Files
#include <gl/GL.h>

//Link with openGL import library
#pragma comment(lib, "opengl32.lib")

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL bFullscreen = FALSE;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;
FILE* gpFile = NULL;
BOOL bActiveWindow = FALSE;
BOOL bEscapeKeyIsPressed = FALSE;

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declarations
	int initialize(void);
	void render(void);
	void update(void);
	void uninitialize(void);

	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;
	MSG msg;
	TCHAR lpszAppName[] = TEXT("RTR7_SVN");
	BOOL bDone = FALSE;

	//create LogFile
	gpFile = fopen("Log.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Creation Failed !!!"), TEXT("ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program Started Successfully!!!\n");
	}

	// code

	// WNDCLASSEX initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = lpszAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Register above WNCLASS
	RegisterClassEx(&wndclass);

	//Centering
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		lpszAppName,
		TEXT("Satvik Vijay Nagare"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		ScreenWidth / 2 - WIN_WIDTH / 2,
		ScreenHeight / 2 - WIN_HEIGHT / 2,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set global handle
	ghwnd = hwnd;

	int iResult = initialize();
	if (iResult != 0)
	{
		fprintf(gpFile, "SVN : Initialized Function Failed!!!\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "SVN : Initialized Function Succeded!!!\n");
	}

	// show window
	ShowWindow(hwnd, iCmdShow);

	// update the window to paint it's background
	UpdateWindow(hwnd);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//game loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if ((bActiveWindow == TRUE))
			{
				if (bEscapeKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}

				render();
				update();
			}
		}
	}

	//Uninitialize
	uninitialize();

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	// local function declarations
	void resize(int, int);
	void uninitialize(void);
	void toggleFullscreen(void);

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		memset(&wpPrev, 0, sizeof(WINDOWPLACEMENT));
		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	case WM_SETFOCUS:
		bActiveWindow = TRUE;
		break;
	case WM_KILLFOCUS:
		bActiveWindow = FALSE;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			bEscapeKeyIsPressed = TRUE;
			break;
		default:
			break;
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case'f':
			if (bFullscreen == FALSE)
			{
				toggleFullscreen();
				bFullscreen = TRUE;
			}
			else
			{
				toggleFullscreen();
				bFullscreen = FALSE;
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		uninitialize();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void toggleFullscreen(void)
{
	// code
	// Variable declaration
	MONITORINFO mi;

	if (bFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			memset(&mi, 0, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);
			// F = Flag
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
}

int initialize(void)
{

	//function declarations
	void resize(int, int);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		return(-1);
	}

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		return(-2);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-3);
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		return(-4);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-5);
	}

	//Choose screen clearing color as black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void render(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	//Do double buffering
	SwapBuffers(ghdc);
}

void update(void)
{
	//code

}

void uninitialize(void)
{
	//code

	//if exitting in full screen 1st restore and then proecced.
	if (bFullscreen == TRUE)
	{
		toggleFullscreen();
		bFullscreen = FALSE;
	}

	//1st check the current context and if it is then make it uncurrent
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	//now destroy the rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	//destroy the device context
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	//close log file
	if (gpFile)
	{
		fprintf(gpFile, "Program Terminated Succesfully!!!\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
