#include "stdafx.h"
#include "resource.h"
#include "Timer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CALLBACK Update(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
	srand((unsigned int)time(NULL));

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass{};

	LPCTSTR lpszClass = L"Window Class Name";
	LPCTSTR lpszWindowName = L"Pokemon Flight";

	static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	static ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW | WS_BORDER,
		500,
		100,
		WINDOWSIZE_X,
		WINDOWSIZE_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, 1, 1, Update);

	Timer::Inst()->Reset();
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static std::wstring frameRate{};
	RECT rect = { 0, 0, 100, 100 };

	switch (uMsg)
	{
	case WM_CREATE:
	{
	}
	break;
	case WM_ERASEBKGND:
		return FALSE;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		Timer::Inst()->GetFrameRate(frameRate);
		DrawText(hdc, frameRate.c_str(), _tcslen(frameRate.c_str()), &rect, DT_TOP | DT_LEFT);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK Update(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	static int count = 0;
	Timer::Inst()->Tick(60.f);
	InvalidateRect(hWnd, NULL, FALSE);
}