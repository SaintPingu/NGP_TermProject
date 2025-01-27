#pragma once
#include "Scene.h"

class LobbyPlayer {
private:
	int mID{};

public:
	Vector2 befpos{};
	Vector2 pos = { 60, 232 };

	bool isMoving;

	Dir dir{ Dir::Down };

	RECT GetRect() const;
	void Move();

	void SetID(int id) { mID = id; }
	int GetID() const { return mID; }
};

class LobbyScene : public Scene {
private:
	std::unordered_map<int, std::shared_ptr<LobbyPlayer>> players{};


	bool CheckMoveScene(const std::shared_ptr<LobbyPlayer>& player);
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<LobbyPlayer>>& GetPlayers() const { return players; }

	void AddClient(int clientID); 
	void RemoveClient(int clientID);

	bool CheckCollision(const std::shared_ptr<LobbyPlayer>& palyer);
	const std::shared_ptr<LobbyPlayer>& GetPlayer(int id);
};