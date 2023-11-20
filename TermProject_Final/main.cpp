#include "stdafx.h"
#include "resource.h"
#include "InputManager.h"
#include "Framework.h"
#include "ClientNetMgr.h"


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

	framework->Start(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, 1, 1, Update);

	Timer::Inst()->Reset();
	InputManager::Inst()->Init();

	// 2023-11-20-MON (장재문) - 클라이언트 네트워크 구동 쓰레드 생성
	std::thread ClientNetwork([&]() {
		CLIENT_NETWORK->Execute();
		});

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	framework->Terminate();
	ClientNetwork.join();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
		framework->Render();
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
	Timer::Inst()->Tick(60.f);

	InputManager::Inst()->Update();
	framework->Update();

	InvalidateRect(hWnd, NULL, TRUE);
}