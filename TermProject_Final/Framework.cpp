#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"

SINGLETON_PATTERN_DEFINITION(Framework)

extern bool isGenPacket;
extern int keyList[];

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
	if (isGenPacket) {
		//WaitForSingleObject(recvpacket?event,INFINITY);
		isGenPacket = false;
	}
}

void Framework::ProcessCommand()
{
}

void Framework::WriteData()
{
}

void Framework::GetInput()
{
	static UCHAR pKeysBuffer[256];
	GetKeyboardState(pKeysBuffer);

	if (sceneManager->GetCurrentScene() == std::make_shared<SceneLobby>()) {

		if (pKeysBuffer[VK_UP] & 0xF0) {
			BYTE dir = int(Dir::Up) -1 ;
			cmdList.CommandPush(dir, NULL, 0);
		}
		else if (pKeysBuffer[VK_DOWN] & 0xF0) {
			BYTE dir = int(Dir::Down) - 1;
			cmdList.CommandPush(dir, NULL, 0);
		}
		else if (pKeysBuffer[VK_LEFT] & 0xF0) {
			BYTE dir = int(Dir::Left) - 1;
			cmdList.CommandPush(dir, NULL, 0);
		}
		else if (pKeysBuffer[VK_RIGHT] & 0xF0) {
			BYTE dir = int(Dir::Right) - 1;
			cmdList.CommandPush(dir, NULL, 0);
		}

	}

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
