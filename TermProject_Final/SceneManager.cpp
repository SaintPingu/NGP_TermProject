#include "stdafx.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "Loading.h"

SINGLETON_PATTERN_DEFINITION(SceneManager)

void SceneManager::Init(const HWND& hWnd)
{
	GetClientRect(hWnd, &rectClientWindow);
	LoadScene(SceneType::Intro);
	loading = std::make_shared<Loading>();
}

void SceneManager::StartRender(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap)
{
	hdc = BeginPaint(hWnd, &ps);
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rectWindow.right, rectWindow.bottom);
	SelectObject(memDC, hBitmap);
	SelectObject(memDC, GetStockObject(WHITE_BRUSH));
	Rectangle(memDC, 0, 0, rectWindow.right, rectWindow.bottom);
	SetStretchBltMode(hdc, COLORONCOLOR);
	SetStretchBltMode(memDC, COLORONCOLOR);
}

void SceneManager::FinishRender(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap)
{
	BitBlt(hdc, 0, 0, rectWindow.right, rectWindow.bottom, memDC, 0, 0, SRCCOPY);
	ValidateRect(hWnd, NULL);
	DeleteDC(memDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
}

void SceneManager::LoadScene(SceneType scene)
{
	switch (scene) {
	case SceneType::Intro:
		crntScene = std::make_shared<SceneIntro>();
		break;
	default:
		assert(0);
		break;
	}

	crntScene->Init();
}

void SceneManager::RenderScene(const HWND& hWnd)
{
	PAINTSTRUCT ps{};
	HDC hdc{}, memDC{};
	HBITMAP hBitmap{};

	StartRender(hWnd, ps, hdc, memDC, hBitmap);
	crntScene->Render(memDC);
	loading->Render(memDC);
	FinishRender(hWnd, ps, hdc, memDC, hBitmap);
}

void SceneManager::AnimateScene()
{
	crntScene->Animate();
	loading->Animate();
}