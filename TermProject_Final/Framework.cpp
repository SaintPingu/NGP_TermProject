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

// SceneStage는 WriteData를 수행하지 않는다.
void Framework::UpdateWithServer_Stage()
{
	AnimateScene();
	if (!SceneMgr->IsLoading()) {
		const std::shared_ptr<SceneStage>& sceneStage = std::static_pointer_cast<SceneStage>(SceneMgr->GetCurrentScene());
		if (sceneStage->IsRecvPacket()) {	// 패킷을 수신해야 한다면
			if (WaitForPacket_Stage()) {	// 패킷 수신 대기
				// 패킷 수신됨
				ProcessCommand();
			}
		}

		GetInput();
		if (sceneStage->IsSendPacket()) {	// 패킷을 송신해야 한다면
			SendPacket();					// 패킷 송신
			sceneStage->SendComplete();		// 패킷 송신 완료
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

// 네트워크 없이 단독 실행
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

	// 시그널 상태(SetEvent)
	if (result == WAIT_OBJECT_0) {
		ResetEvent(recvPacket);

		if (CLIENT_NETWORK->IsConnected() == false) {
			return false;
		}

		packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetPacketBuffer());
		if (packetLoader.buffer->empty()) {	// 예외 처리
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
		std::cout << "서버 연결시간 초과\n";
		Sleep(2000);
		Terminate();
		return false;
	}
	ResetEvent(recvPacket);

	if (CLIENT_NETWORK->IsConnected() == false) {
		return false;
	}

	packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetPacketBuffer());
	if (packetLoader.buffer->empty()) {	// 예외 처리
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
	//packetLoader를 통해 서버의 Data를 처리한다
	PacketBuffer buffer = packetLoader.PopData();

	// 예외 처리
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

	// 서버 연결 대기
	constexpr int timeoutMSec = 10 * 1000;
	DWORD result = WaitForSingleObject(serverConnect, timeoutMSec);

	if (result == WAIT_TIMEOUT || CLIENT_NETWORK->IsConnected() == false) {
		MessageBox(hWnd, L"서버 연결에 실패하였습니다.", L"연결 에러", MB_ICONERROR | MB_OK);
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
