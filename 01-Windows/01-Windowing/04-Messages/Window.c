// header Files
#include <windows.h>

#include "window.h"

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;
	MSG msg;
	TCHAR lpszAppName[] = TEXT("RTR7_SVN");

	// code

	// WNDCLASSEX initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	hwnd = CreateWindow(lpszAppName,
		TEXT("Satvik Vijay Nagare"),
		WS_OVERLAPPEDWINDOW,
		ScreenWidth / 2 - WIN_WIDTH / 2,
		ScreenHeight / 2 - WIN_HEIGHT / 2,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

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
	TCHAR str[255];
	wsprintf(str, TEXT("%d"), (int)msg.wParam);
	MessageBox(NULL, str, TEXT("WPARAM"), MB_OK);
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// code
	switch (iMsg)
	{
	case WM_CREATE:
		MessageBox(hwnd, TEXT("WM_CREATE SIZE IS RECIEVED"), TEXT("MESSAGE"), MB_OK);
		break;
	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_SIZE:
		MessageBox(hwnd, TEXT("WM_SIZE SIZE IS RECIEVED"), TEXT("MESSAGE"), MB_OK);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			MessageBox(hwnd, TEXT("WM_KEYDOWN : ESCAPE KEY IS PRESSED"), TEXT("MESSAGE"), MB_OK);
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
			MessageBox(hwnd, TEXT("WM_CHAR : F KEY IS PRESSED"), TEXT("MESSAGE"), MB_OK);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		MessageBox(hwnd, TEXT("WM_CLOSE IS RECIEVED"), TEXT("MESSAGE"), MB_OK);
		break;
	case WM_DESTROY:
		MessageBox(hwnd, TEXT("WM_DESTROY IS RECIEVED"), TEXT("MESSAGE"), MB_OK);
		PostQuitMessage(29);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

