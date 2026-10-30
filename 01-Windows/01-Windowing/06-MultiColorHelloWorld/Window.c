// header Files
#include <windows.h>

#include "window.h"

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
int IPaintFlag = -1;

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
	
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//variable declarations
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	TCHAR str[] = TEXT("Hello World !!!");
	
	// code
    switch (iMsg)
	{
	case WM_CREATE:
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
			break;
		case 'R':
		case 'r':
			IPaintFlag = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'G':
		case 'g':
			IPaintFlag = 2;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'B':
		case 'b':
			IPaintFlag = 3;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'C':
		case 'c':
			IPaintFlag = 4;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'M':
		case 'm':
			IPaintFlag = 5;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'Y':
		case 'y':
			IPaintFlag = 6;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		default:
			IPaintFlag = -1;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, RGB(0, 0, 0));
		if (IPaintFlag == 1)
			SetTextColor(hdc, RGB(255, 0, 0));
		else if (IPaintFlag == 2)
				SetTextColor(hdc, RGB(0, 255, 0));
		else if (IPaintFlag == 3)
					SetTextColor(hdc, RGB(0, 0, 255));
		else if (IPaintFlag == 4)
					SetTextColor(hdc, RGB(0, 255, 255));
		else if (IPaintFlag == 5)
					SetTextColor(hdc, RGB(255, 0, 255));
		else if (IPaintFlag == 6)
					SetTextColor(hdc, RGB(255, 255, 0));
		else
					SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
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

