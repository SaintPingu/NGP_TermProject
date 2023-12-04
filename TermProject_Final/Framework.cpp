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
	soundManager = std::make_shared<SoundManager>();


	sceneManager = std::make_shared<SceneManager>();
	soundManager = std::make_shared<SoundManager>();
	sceneManager->Init(hWnd);

	this->hWnd = hWnd;

	SetUpdateFuncToSingle();
}

void Framework::UpdateWithServer()
{
	if (!SceneMgr->IsLoading()) {
		if (WaitForPacket() == false) {
			return;
		}
		if (!packetLoader.buffer->empty()) {
			if (ProcessCommand() == false) {
				return;
			}
			WriteData();
		}

		if (!SceneMgr->IsLoading()) {
			GetInput();
			SendPacket();
		}
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


bool Framework::WaitForPacket()
{
RestartRecv:
	//std::cout << " Framework 수신 대기\n";
	constexpr int timeoutMSec = 5 * 1000;
	DWORD result = WaitForSingleObject(recvPacket, timeoutMSec);
	if (result == WAIT_TIMEOUT) {
		std::cout << "서버 연결시간 초과\n";
		Sleep(2000);
		Terminate();
		return false;
	}
	ResetEvent(recvPacket);
	//std::cout << " Framework 수신 완료\n";

	if (CLIENT_NETWORK->IsConnected() == false) {
		return false;
	}
	else if (CLIENT_NETWORK->GetConnectFlag() != ConnectFlag::RecvFinish) {
		// Stage에서 왜 WakeForPacket 함수가 호출되지 않았는데도 실행되는가...
		goto RestartRecv;
	}

	packetLoader.SetPacketBuffer(CLIENT_NETWORK->GetPacketBuffer());
	if (packetLoader.buffer->empty()) {	// 예외 처리
		std::cout << "[ERROR - WaitForPacket()] :: 수신한 패킷이 비어있습니다!\n";
		return false;
	}
	//std::cout << "패킷 정상 수신 / 데이터 크기 : (" << packetLoader.buffer->size() << ")\n";

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
	ResetEvent(recvPacket);
	CLIENT_NETWORK->Send();
}

void Framework::ExitStage()
{
	// 스테이지에서 벗어나 로비로 입장 전, Stop 커맨드를 한 번 보낸다. (동기화)
	//CommandList* cmdList = &CLIENT_NETWORK->GetPacketGenerator().cmdList;
	//cmdList->PushCommand((BYTE)ClientLobbyCmd::Stop, nullptr, 0);
}
