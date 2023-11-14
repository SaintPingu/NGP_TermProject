#pragma once
#include "Scene.h"

class LobbyPlayer {
public:
	POINT pos = { 60, 232 };
	POINT aboutMapPos = { pos.x, pos.y };
	RECT rect = { 0, };
	Dir dir = Dir::Empty;
	bool isMoving = false;

	void Move();
};

class LobbyScene : public Scene {
private:
	std::unordered_map<int, std::shared_ptr<LobbyPlayer>> players{};

public:
	virtual void Init() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;
	void Update();
};