#pragma once
#include "ClientPacket.h"
#include "SceneManager.h"

//class SceneManager;
class ClientNetwork;

class Framework {
	SINGLETON_PATTERN(Framework)

private:
	//패킷을 서버로부터 받았다는 이벤트
	HANDLE recvPacket;
	HANDLE serverConnect;
private:
	std::thread clientNetwork{};
	RECT rectClientWindow{};
	HWND hWnd;
	std::shared_ptr<SceneManager> sceneManager{};
	//std::shared_ptr<ClientNetwork> clientNetwork{};


	

	PacketLoader packetLoader;

	bool WaitForPacket();
	void ProcessCommand();
	void WriteData();
	void SendPacket();
	void AnimateScene();

	void UpdateWithServer();
	void UpdateSingle();
	void SetUpdateFuncToServer() { UpdateFunc = std::bind(&Framework::UpdateWithServer, this); }
	void SetUpdateFuncToSingle() { UpdateFunc = std::bind(&Framework::UpdateSingle, this); }

public:
	void GetInput();

	const RECT& GetRectWindow() { return rectClientWindow; }

	void Start(HWND hWnd);
	
	void Update();
	void Render();
	void Terminate();

	void ConnectToServer();
	void DisconnectServer();
	void CompleteServerConnect() { SetEvent(serverConnect); }
	
	// 23-11-16 최정일 클라이언트 ID를 알아야 움직일 플레이어가 누구이고 화면에 표시할 중심 캐릭터를 알수있음.
	int client_ID = 0;
	void SetClientID(int id) { client_ID = id; }
	SceneManager* GetSceneMgr() { 	return sceneManager.get(); }

	void SetPacketLoader() { packetLoader; }
	PacketLoader& GetPacketLoader() { return packetLoader; }

	// 23-11-21 최정일 시작시 아무것도 없는 데이터 통신필요
	void DefaultPacketSend();

	// 23-11-25 민동현 : WaitForPacket() 함수를 꺠우기 위한 함수
	void WakeForPacket() { SetEvent(recvPacket); }


private:
	std::function<void()> UpdateFunc{};

};

#define framework Framework::Inst()
#define SceneMgr Framework::Inst()->GetSceneMgr()
#define CrntScene SceneMgr->GetCurrentScene()
