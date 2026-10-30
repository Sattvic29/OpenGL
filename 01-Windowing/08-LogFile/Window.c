// header Files
#include <windows.h>
#include <stdio.h>
#include "window.h"
#include <stdlib.h>

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
HWND ghwnd = NULL;
BOOL bFullscreen = FALSE;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;
FILE* gpFile = NULL;

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;
	MSG msg;
	TCHAR lpszAppName[] = TEXT("RTR7_SVN");

	//create LogFile
	gpFile = fopen("Log.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Creation Failed !!!"), TEXT("ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program Started Succesfully!!!\n");
	}

	// code

	// WNDCLASSEX initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
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

	// show window
	ShowWindow(hwnd, iCmdShow);

	// update the window to paint it's background
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	//close log file
	if (gpFile)
	{
		fprintf(gpFile, "Program Terminated Succesfully!!!\n");
		fclose(gpFile);
        gpFile = NULL;
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	// local function declarations
	void toggleFullscreen(void);
	
	// code
	switch (iMsg)
	{
	case WM_CREATE:
		memset(&wpPrev, 0, sizeof(WINDOWPLACEMENT));
		wpPrev.length = sizeof(WINDOWPLACEMENT);
		break;
	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			break;
		default :
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
