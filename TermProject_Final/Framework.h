#pragma once
#include "ClientPacket.h"
#include "SceneManager.h"

//class SceneManager;
class ClientNetwork;

class Framework {
	SINGLETON_PATTERN(Framework)

private:
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
public:
	void GetInput();

	const RECT& GetRectWindow() { return rectClientWindow; }

	void Start(HWND hWnd);
	void Update();
	void Render();
	void Terminate();
	
	// 23-11-16 ������ Ŭ���̾�Ʈ ID�� �˾ƾ� ������ �÷��̾ �����̰� ȭ�鿡 ǥ���� �߽� ĳ���͸� �˼�����.
	int client_ID = 0;
	SceneManager* GetSeceneMgr() { 	return sceneManager.get(); }

	void SetPacketLoader() { packetLoader; }

	// 23-11-21 ������ ���۽� �ƹ��͵� ���� ������ ����ʿ�
	void DefaultPacketSend();
};

#define framework Framework::Inst()
#define rectWindow framework->GetRectWindow()