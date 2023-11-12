#pragma once

class SceneManager;
class ClientNetwork;
class PacketLoader;

class Framework {
	SINGLETON_PATTERN(Framework)

private:
	RECT rectClientWindow{};
	HWND hWnd;
	std::shared_ptr<SceneManager> sceneManager{};
	//std::shared_ptr<ClientNetwork> clientNetwork{};

	void WaitForPacket();
	void ProcessCommand();
	void WriteData();
	void GetInput();
	void SendPacket();
	void AnimateScene();
public:
	const RECT& GetRectWindow() { return rectClientWindow; }

	void Start(HWND hWnd);
	void Update();
	void Render();
	void Terminate();
};

#define framework Framework::Inst()
#define rectWindow framework->GetRectWindow()