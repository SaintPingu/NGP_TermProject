#pragma once
#include "Scene.h"

class LobbyPlayer {
public:
	POINT befpos{};
	POINT pos = { 60, 232 };

	bool isMoving;

	Dir dir{ Dir::Down };

	RECT GetRect();
	void Move();
};

class LobbyScene : public Scene {
private:
	std::unordered_map<int, std::shared_ptr<LobbyPlayer>> players{};

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<LobbyPlayer>>& GetPlayers() const { return players; }

	void AddPlayer(int clientID) { players[clientID] = std::make_shared<LobbyPlayer>(); }

	bool CheckCollision(RECT playerRect);
};