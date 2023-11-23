#include "stdafx.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "Loading.h"
#include "Framework.h"

void SceneManager::Init(HWND hWnd)
{
	//LoadScene(SceneType::Intro);
	LoadScene(SceneType::Lobby);
	loading = std::make_shared<Loading>();
}

void SceneManager::StartRender(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap)
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

void SceneManager::FinishRender(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap)
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
	case SceneType::Lobby:
		crntScene = std::make_shared<SceneLobby>();
		break;
	default:
		assert(0);
		break;
	}

	crntScene->Init();
	crntSceneType = scene;
}

void SceneManager::RenderScene(HWND hWnd)
{
	PAINTSTRUCT ps{};
	HDC hdc{}, memDC{};
	HBITMAP hBitmap{};

	StartRender(hWnd, ps, hdc, memDC, hBitmap);
	crntScene->Render(memDC);
	//loading->Render(memDC);
	FinishRender(hWnd, ps, hdc, memDC, hBitmap);
}

void SceneManager::AnimateScene()
{
	crntScene->Animate();
	loading->Animate();
}

bool SceneManager::IsLoading()
{
	return false;
}
