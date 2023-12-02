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

	SetUpdateFuncToSingle();
}

void Framework::UpdateWithServer()
{
	if (WaitForPacket() == false) {
		return;
	}
	ProcessCommand();
	WriteData();
	GetInput();
	SendPacket();
	AnimateScene();
}

// ��Ʈ��ũ ���� �ܵ� ����
void Framework::UpdateSingle()
{
	GetInput();
	AnimateScene();
}


void Framework::Update()
{
	UpdateSingle(); 
	//UpdateFunc();
	
}

bool Framework::WaitForPacket()
{
	WaitForSingleObject(recvPacket, INFINITE);
	ResetEvent(recvPacket);

	if (CLIENT_NETWORK->IsConnected() == false) {
		return false;
	}

	packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetPacketBuffer());
	if (packetLoader.buffer->empty()) {	// ���� ó��
		SetEvent(recvPacket);
		return false;
	}
	return true;
}

void Framework::ProcessCommand()
{
	CrntScene->ProcessCommand();
}

void Framework::WriteData()
{
	//packetLoader�� ���� ������ Data�� ó���Ѵ�
	PacketBuffer buffer = packetLoader.PopData();

	// ���� ó��
	if (buffer.empty()) {
		SetEvent(recvPacket);
		return;
	}

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
	//���� ���� IntroScene�� �ƴ϶�� thread for server�� �����
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
	if (clientNetwork.joinable()) {
		clientNetwork.join();
	}
}

void Framework::ConnectToServer()
{
	if (clientNetwork.joinable()) {
		clientNetwork.join();
	}

	clientNetwork = std::thread([&]() {
		CLIENT_NETWORK_MGR->Execute();
		});

	// ���� ���� ���
	WaitForSingleObject(serverConnect, INFINITE);

	if (CLIENT_NETWORK->IsConnected() == false) {
		MessageBox(hWnd, L"���� ���ῡ �����Ͽ����ϴ�.", L"���� ����", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
		return;
	}

	SetUpdateFuncToServer();
}

void Framework::DisconnectServer()
{
	SetUpdateFuncToSingle();
	SceneMgr->DisConnect();
}

void Framework::DefaultPacketSend()
{
	CLIENT_NETWORK->Send();
}
