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
	SceneType crntScene = sceneManager->GetCurrentScene()->Identify();
	//packetLoader를 통해 서버의 Command를 처리한다
	BYTE cmd;
	PacketBuffer buffer;
	if (crntScene == SceneType::Lobby) {
		// 로비는 명령이 항상 1개 이므로 바로 반복 필요x
		packetLoader.PopCommand(cmd, buffer, SceneType::Lobby);
		switch (cmd)
		{
		case (BYTE)ServerLobbyCmd::GoMenu:
			sceneManager->LoadScene(SceneType::Intro);
			break;
		case (BYTE)ServerLobbyCmd::GoStage:
			sceneManager->LoadScene(SceneType::Stage);
			break;
		case (BYTE)ServerLobbyCmd::None:
			// 아무것도 하지 않는 명령어를 의미
			break;
		case (BYTE)ServerLobbyCmd::Quit:
			PostQuitMessage(0);
			// Terminate 명령
			break;
		default:
			break;
		}
	}
	/*else if (crntScene == SceneType::Stage) {
		while (true) {
			if (!packetLoader.PopCommand(cmd, buffer, SceneType::Stage)) {
				break;
			}
		}
	}
	else if (crntScene == SceneType::Battle) {
		while (true) {
			if (!packetLoader.PopCommand(cmd, buffer, SceneType::Battle)) {
				break;
			}
		}
	}*/

}

void Framework::WriteData()
{
	SceneType crntScene = sceneManager->GetCurrentScene()->Identify();

	//packetLoader를 통해 서버의 Command를 처리한다
	PacketBuffer buffer = packetLoader.PopData();

	if (crntScene == SceneType::Lobby) {
		sceneManager->GetCurrentScene()->WriteData(&buffer);
	}
	else if (crntScene == SceneType::Stage) {
	
	}
	else if (crntScene == SceneType::Battle) {
	
	}
}

void Framework::GetInput()
{
	SceneType crntScene = sceneManager->GetCurrentScene()->Identify();

	if (crntScene == SceneType::Lobby) {

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
	else if (crntScene == SceneType::Stage) {

	}
	else if (crntScene == SceneType::Battle) {

	}

}

void Framework::SendPacket()
{
	SceneType crntScene = sceneManager->GetCurrentScene()->Identify();
	//현재 씬이 IntroScene이 아니라면 thread for server를 깨운다
	if (crntScene != SceneType::Intro) {
		SetEvent(wakeUpThreadForServer);
	}

}

void Framework::AnimateScene()
{
	GetInput();
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
