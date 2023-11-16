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

	CommandList cmdList;

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
	
	// 23-11-16 클라이언트 ID를 알아야 움직일 플레이어가 누구이고 화면에 표시할 중심 캐릭터를 알수있음.
	int client_ID = 0;
	SceneManager* GetSeceneMgr() { 	return sceneManager.get(); }
};

#define framework Framework::Inst()
#define rectWindow framework->GetRectWindow()