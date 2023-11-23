#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "InputManager.h"
#include "ClientNetMgr.h"
#include "ClientNetwork.h"

SINGLETON_PATTERN_DEFINITION(Framework)

//��Ŷ�� �����κ��� �޾Ҵٴ� �̺�Ʈ, ��Ʈ�ξ����� �κ�� �Ѿ�� ������ ����� �̺�Ʈ
HANDLE recvPacket, wakeUpThreadForServer;

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
	//std::cout << "update" << std::endl;
	//WaitForPacket();
	//ProcessCommand();
	//WriteData();
	GetInput();
	//SendPacket();
	AnimateScene();
}

void Framework::WaitForPacket()
{
	if (isGenPacket) {
		WaitForSingleObject(recvPacket,INFINITY);
		packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetClientNetwork()->GetPacketBuffer());
		isGenPacket = false;
	}
}

void Framework::ProcessCommand()
{
	CrntScene->ProcessCommand();
}

void Framework::WriteData()
{
	//packetLoader�� ���� ������ Data�� ó���Ѵ�
	PacketBuffer buffer = packetLoader.PopData();

	CrntScene->WriteData(&buffer);
}

void Framework::GetInput()
{
	ClientNetwork* network = CLIENT_NETWORK->GetClientNetwork();
	if (!network) {
		return;
	}

	CommandList* cmdList = network->GetPacketGenerator().GetCommandList();
	CrntScene->GetInput(cmdList);
}

void Framework::SendPacket()
{
	//���� ���� IntroScene�� �ƴ϶�� thread for server�� �����
	if (SceneMgr->GetCurrentSceneType() == SceneType::Intro) {
		return;
	}

	SetEvent(wakeUpThreadForServer);

	CLIENT_NETWORK->GetClientNetwork()->SendPacket();

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

void Framework::DefaultPacketSend()
{
	CLIENT_NETWORK->GetClientNetwork()->SendPacket();
}
