#pragma once
#include "Scene.h"

class LobbyPlayer {
	POINT aboutMapPos;
	POINT pos = { 0, };
	RECT rectDraw = { 0, };
	RECT rectImage = { 0, };
	Dir dir = Dir::Down;
	bool isMoving = false;
};

class LobbyScene : public Scene {
private:
	std::unordered_map<int, LobbyPlayer> players{};

public:
	virtual void Init() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;
};