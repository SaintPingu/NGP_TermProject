#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"


SINGLETON_PATTERN_DEFINITION(Framework)



void Framework::Start(HWND hWnd)
{
	GetClientRect(hWnd, &rectClientWindow);
	sceneManager = std::make_shared<SceneManager>();
	sceneManager->Init(hWnd);

	this->hWnd = hWnd;
}

void Framework::Update()
{
	AnimateScene();
}

void Framework::WaitForPacket()
{
}

void Framework::ProcessCommand()
{
}

void Framework::WriteData()
{
}

void Framework::GetInput()
{
}

void Framework::SendPacket()
{
}

void Framework::AnimateScene()
{
	sceneManager->AnimateScene();
}

void Framework::Render()
{
	sceneManager->RenderScene(hWnd);
}

void Framework::Terminate()
{
}
