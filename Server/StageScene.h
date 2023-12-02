#pragma once
#include "Scene.h"

const int maxStagePlayer = 2;
class StagePlayer
{
public:
	int ID{};
	Type typeFly = Type::Empty;
	Type typeGnd = Type::Empty;

public:
	void PushCommand(ServerStageCmd cmd, Type other_fly, Type other_gnd);

};

class StageScene : public Scene
{
private:
	bool battleReady = false;
	int  curPlayerCnt = 0;
	std::unordered_map<int, std::shared_ptr<StagePlayer>> players{};

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<StagePlayer>>& GetPlayers() const { return players; }

	bool AddPlayer(int clientID);
	void Clear();

};

