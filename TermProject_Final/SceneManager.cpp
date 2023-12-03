#include "stdafx.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "SceneStage.h"
#include "SceneBattle.h"
#include "Loading.h"
#include "Framework.h"

void SceneManager::Init(HWND hWnd)
{
	InitScene(SceneType::Intro);
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

void SceneManager::InitScene(SceneType scene)
{
	if (crntSceneType == SceneType::Intro && scene != SceneType::Intro) {
		framework->ConnectToServer();
	}

	if (crntSceneType == SceneType::Stage) {
		framework->ExitStage();
	}

	switch (scene) {
	case SceneType::Intro:
		crntScene = std::make_shared<SceneIntro>();
		break;
	case SceneType::Lobby:
		crntScene = std::make_shared<SceneLobby>();
		break;
	case SceneType::Stage:
		crntScene = std::make_shared<SceneStage>();
		framework->EnterStage();
		break;
	case SceneType::Battle:
		crntScene = std::make_shared<SceneBattle>();
		break;
	default:
		assert(0);
		break;
	}

	crntScene->Init();
	crntSceneType = scene;
}

void SceneManager::LoadScene(SceneType scene)
{
	loading->ResetLoading();
	nextSceneType = scene;
}

void SceneManager::RenderScene(HWND hWnd)
{
	PAINTSTRUCT ps{};
	HDC hdc{}, memDC{};
	HBITMAP hBitmap{};

	StartRender(hWnd, ps, hdc, memDC, hBitmap);
	crntScene->Render(memDC);
	if (IsLoading()) {
		loading->Render(memDC);
	}
	FinishRender(hWnd, ps, hdc, memDC, hBitmap);
}

void SceneManager::AnimateScene()
{
	crntScene->Animate();
	if (IsLoading()) {
		loading->Animate();

		if (loading->IsLoaded()) {
			InitScene(nextSceneType);
		}
	}
}

void SceneManager::DisConnect()
{
	LoadScene(SceneType::Intro);
}

bool SceneManager::IsLoading()
{
	return !loading->IsLoaded();
}
