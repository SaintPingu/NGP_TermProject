#pragma once
#include "Scene.h"

class LobbyPlayer {
public:
	POINT pos = { 60, 232 };
	POINT aboutMapPos = { pos.x, pos.y };
	RECT rect = { 0, };

	int v{}, h{};

	Dir befDir{ Dir::Down };

	void Move();
	bool IsMoving();
	Dir GetDir();
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
};