#pragma once
#include "ClientPacket.h"
#include "SceneManager.h"

//class SceneManager;
class ClientNetwork;

class Framework {
	SINGLETON_PATTERN(Framework)

private:
	//��Ŷ�� �����κ��� �޾Ҵٴ� �̺�Ʈ
	HANDLE recvPacket;
	HANDLE serverConnect;

private:
	std::thread clientNetwork{};
	RECT rectClientWindow{};
	HWND hWnd;
	std::shared_ptr<SceneManager> sceneManager{};
	//std::shared_ptr<ClientNetwork> clientNetwork{};


	

	PacketLoader packetLoader;

	void WaitForPacket();
	void ProcessCommand();
	void WriteData();
	void SendPacket();
	void AnimateScene();

	void UpdateWithServer();
	void UpdateSingle();

public:
	void GetInput();

	const RECT& GetRectWindow() { return rectClientWindow; }

	void Start(HWND hWnd);
	
	void Update();
	void Render();
	void Terminate();

	void ConnectToServer();
	void CompleteServerConnect() { SetEvent(serverConnect); }
	
	// 23-11-16 ������ Ŭ���̾�Ʈ ID�� �˾ƾ� ������ �÷��̾ �����̰� ȭ�鿡 ǥ���� �߽� ĳ���͸� �˼�����.
	int client_ID = 0;
	SceneManager* GetSceneMgr() { 	return sceneManager.get(); }

	void SetPacketLoader() { packetLoader; }
	PacketLoader& GetPacketLoader() { return packetLoader; }

	// 23-11-21 ������ ���۽� �ƹ��͵� ���� ������ ����ʿ�
	void DefaultPacketSend();

	// 23-11-25 �ε��� : WiatForPacket() �Լ��� �ƿ�� ���� �Լ�
	void WakeForPacket() { SetEvent(recvPacket); }

private:
	std::function<void()> UpdateFunc{ std::bind(&Framework::UpdateSingle, this) };

};

#define framework Framework::Inst()
#define SceneMgr Framework::Inst()->GetSceneMgr()
#define CrntScene SceneMgr->GetCurrentScene()
