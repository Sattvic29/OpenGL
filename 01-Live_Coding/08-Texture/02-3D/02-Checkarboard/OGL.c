// header Files
#include <windows.h>
#include <stdio.h>
#include "OGL.h"
#include <stdlib.h>

//OpenGL header Files
#include <gl/GL.h>
#include <gl/glu.h>

//Link with openGL import library
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HEIGHT 64

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

GLuint texture_checkerboard = 0;
GLubyte checkImage[CHECK_IMAGE_HEIGHT][CHECK_IMAGE_HEIGHT][4];

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
	void loadGLTexture(GLuint*);

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

	//Enable depth
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Choose screen clearing color as grey
	glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

	//Load textures
	loadGLTexture(&texture_checkerboard);

	//Enable Texturing
	glEnable(GL_TEXTURE_2D);

	//Warmup resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void loadGLTexture(GLuint* tex)
{
	//function prototype
	void makeCheckImage(void);

	makeCheckImage();

	//code
	//1
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//2
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);

	//2
	glBindTexture(GL_TEXTURE_2D, *tex);

	//3
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//5
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECK_IMAGE_WIDTH, CHECK_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)checkImage);

	//6
	glBindTexture(GL_TEXTURE_2D, 0);

	//7
	glBindTexture(GL_TEXTURE_2D, *tex);
	glBindTexture(GL_TEXTURE_2D, *tex);

}

void makeCheckImage(void)
{
	//code
	int i, j, c;

	for (i = 0; i < CHECK_IMAGE_HEIGHT; i++)
	{
		for (j = 0; j < CHECK_IMAGE_WIDTH; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Perspective Projection(from redbook)
	gluPerspective(60.0f,
				   (GLfloat)width / (GLfloat)height,
				   1.0f,
		           30.0f);
}

void render(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.6f);

	glBindTexture(GL_TEXTURE_2D, texture_checkerboard);

	glBegin(GL_QUADS);

	//straight or front face Quad
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	//Quad
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.41421f, 1.0f, -1.41421f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.41421f, -1.0f, -1.41421f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

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

	if (texture_checkerboard)
	{
		glDeleteTextures(1, &texture_checkerboard);
		texture_checkerboard = 0;
	}

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
