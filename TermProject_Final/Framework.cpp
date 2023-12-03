#include "stdafx.h"
#include "Framework.h"
#include "SceneManager.h"
#include "SceneIntro.h"
#include "SceneLobby.h"
#include "InputManager.h"
#include "ClientNetMgr.h"
#include "ClientNetwork.h"
#include "SceneStage.h"

SINGLETON_PATTERN_DEFINITION(Framework)

//#define SINGLEPLAY

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

// SceneStage�� WriteData�� �������� �ʴ´�.
void Framework::UpdateWithServer_Stage()
{
	AnimateScene();
	if (!SceneMgr->IsLoading()) {
		const std::shared_ptr<SceneStage>& sceneStage = std::static_pointer_cast<SceneStage>(SceneMgr->GetCurrentScene());
		if (sceneStage->IsRecvPacket()) {	// ��Ŷ�� �����ؾ� �Ѵٸ�
			if (WaitForPacket_Stage()) {	// ��Ŷ ���� ���
				// ��Ŷ ���ŵ�
				ProcessCommand();
			}
		}

		GetInput();
		if (sceneStage->IsSendPacket()) {	// ��Ŷ�� �۽��ؾ� �Ѵٸ�
			SendPacket();					// ��Ŷ �۽�
			sceneStage->SendComplete();		// ��Ŷ �۽� �Ϸ�
		}
	}
}

void Framework::UpdateWithServer()
{
	if (!SceneMgr->IsLoading()) {
		if (WaitForPacket() == false) {
			return;
		}
		if (ProcessCommand() == false) {
			return;
		}
		WriteData();
		GetInput();
		SendPacket();
	}
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
#ifdef SINGLEPLAY
	UpdateSingle(); 
#else
	UpdateFunc();
#endif // SINGLEPLAY	
}

bool Framework::WaitForPacket_Stage()
{
	constexpr int waitMSec = 100;
	DWORD result = WaitForSingleObject(recvPacket, waitMSec);

	// �ñ׳� ����(SetEvent)
	if (result == WAIT_OBJECT_0) {
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
	// timeout
	else {
		return false;
	}
}
bool Framework::WaitForPacket()
{
	constexpr int timeoutMSec = 5 * 1000;
	DWORD result = WaitForSingleObject(recvPacket, timeoutMSec);
	if (result == WAIT_TIMEOUT) {
		std::cout << "���� ����ð� �ʰ�\n";
		Sleep(2000);
		Terminate();
		return false;
	}
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

bool Framework::ProcessCommand()
{
	return CrntScene->ProcessCommand();
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
	CLIENT_NETWORK->Terminate();
	if (clientNetwork.joinable()) {
		clientNetwork.join();
	}

	PostQuitMessage(0);
}

void Framework::ConnectToServer()
{
#ifdef SINGLEPLAY
	return;
#endif // SINGLEPLAY

	if (clientNetwork.joinable()) {
		clientNetwork.join();
	}

	clientNetwork = std::thread([&]() {
		CLIENT_NETWORK_MGR->Execute();
		});

	// ���� ���� ���
	constexpr int timeoutMSec = 10 * 1000;
	DWORD result = WaitForSingleObject(serverConnect, timeoutMSec);

	if (result == WAIT_TIMEOUT || CLIENT_NETWORK->IsConnected() == false) {
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

void Framework::EnterStage()
{
	SetUpdateFuncToServer_Stage();
}

void Framework::ExitStage()
{
	SetUpdateFuncToServer();
	CommandList* cmdList = &CLIENT_NETWORK->GetPacketGenerator().cmdList;
	cmdList->PushCommand((BYTE)ClientLobbyCmd::Stop, nullptr, 0);
}
