#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "InputManager.h"
#include "ClientNetMgr.h"
#include "ClientNetwork.h"

SINGLETON_PATTERN_DEFINITION(Framework)

void Framework::Start(HWND hWnd)
{
	recvPacket = CreateEvent(NULL, FALSE, FALSE, NULL);
	serverConnect = CreateEvent(NULL, FALSE, FALSE, NULL);

	GetClientRect(hWnd, &rectClientWindow);
	sceneManager = std::make_shared<SceneManager>();
	sceneManager->Init(hWnd);
	this->hWnd = hWnd;
}

void Framework::UpdateWithServer()
{
	WaitForPacket();
	ProcessCommand();
	WriteData();
	GetInput();
	SendPacket();
	AnimateScene();
}

void Framework::UpdateSingle()
{
	GetInput();
	AnimateScene();
}


void Framework::Update()
{
	UpdateFunc();
	
}

void Framework::WaitForPacket()
{
	WaitForSingleObject(recvPacket, INFINITE);
	ResetEvent(recvPacket);

	packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetPacketBuffer());
}

void Framework::ProcessCommand()
{
	CrntScene->ProcessCommand();
}

void Framework::WriteData()
{
	//packetLoader를 통해 서버의 Data를 처리한다
	PacketBuffer buffer = packetLoader.PopData();

	CrntScene->WriteData(&buffer);
}

void Framework::GetInput()
{
	if (!CLIENT_NETWORK) {
		CrntScene->GetInput(nullptr);
		return;
	}

	CommandList* cmdList = &CLIENT_NETWORK->GetPacketGenerator().cmdList;
	CrntScene->GetInput(cmdList);
}

void Framework::SendPacket()
{
	//현재 씬이 IntroScene이 아니라면 thread for server를 깨운다
	if (SceneMgr->GetCurrentSceneType() == SceneType::Intro) {
		return;
	}

	CLIENT_NETWORK->Send();

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
	clientNetwork.join();
}

void Framework::ConnectToServer()
{
	clientNetwork = std::thread([&]() {
		CLIENT_NETWORK_MGR->Execute();
		});

	// 서버 연결 대기
	WaitForSingleObject(serverConnect, INFINITE);

	if (CLIENT_NETWORK->IsConnected() == false) {
		MessageBox(hWnd, L"서버 연결에 실패하였습니다.", L"연결 에러", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
		return;
	}

	UpdateFunc = std::bind(&Framework::UpdateWithServer, this);
}

void Framework::DefaultPacketSend()
{
	CLIENT_NETWORK->Send();
}
