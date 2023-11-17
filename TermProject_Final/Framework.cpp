#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "InputManager.h"

SINGLETON_PATTERN_DEFINITION(Framework)

extern bool isGenPacket;

void Framework::Start(HWND hWnd)
{
	recvPacket = CreateEvent(NULL, FALSE, FALSE, NULL);
	wakeUpThreadForServer = CreateEvent(NULL, FALSE, FALSE, NULL);

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
		WaitForSingleObject(recvPacket,INFINITY);
		isGenPacket = false;
	}
}

void Framework::ProcessCommand()
{
	//PacketDecoder를 통해 서버의 Command를 처리한다
	BYTE cmd;

	packetLoader.PopCommand(cmd, cmdList);
}

void Framework::WriteData()
{
}

void Framework::GetInput()
{
	static UCHAR pKeysBuffer[256];
	GetKeyboardState(pKeysBuffer);

	if (sceneManager->GetCurrentScene() == std::make_shared<SceneLobby>()) {

		if (KEY_PRESSED(VK_UP) || KEY_TAP(VK_UP)) {
			BYTE cmd = BYTE(ClientLobbyCmd::MoveUp);
			cmdList.CommandPush(cmd, NULL, 0);
		}
		else if (KEY_PRESSED(VK_DOWN) || KEY_TAP(VK_DOWN)) {
			BYTE cmd = BYTE(ClientLobbyCmd::MoveDown);
			cmdList.CommandPush(cmd, NULL, 0);
		}
		else if (KEY_PRESSED(VK_LEFT) || KEY_TAP(VK_LEFT)) {
			BYTE cmd = BYTE(ClientLobbyCmd::MoveLeft);
			cmdList.CommandPush(cmd, NULL, 0);
		}
		else if (KEY_PRESSED(VK_RIGHT) || KEY_TAP(VK_RIGHT)) {
			BYTE cmd = BYTE(ClientLobbyCmd::MoveRight);
			cmdList.CommandPush(cmd, NULL, 0);
		}

		if (KEY_TAP(VK_ESCAPE)) {
			BYTE cmd = BYTE(ClientLobbyCmd::Terminate);
			cmdList.CommandPush(cmd, NULL, 0);
		}

	}

}

void Framework::SendPacket()
{
	//현재 씬이 IntroScene이 아니라면 thread for server를 깨운다
	if (sceneManager->GetCurrentScene() == std::make_shared<SceneIntro>()) {
		SetEvent(wakeUpThreadForServer);
	}

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
	CloseHandle(recvPacket);
	CloseHandle(wakeUpThreadForServer);
}
