#pragma once
#include "Scene.h"

const int maxBlattlePlayer = 2;
class BattlePlayer
{
private:
	Dir		curDir;
	bool	isMoving;
	float	speed = 200.f;
	POINT   dirVector{};
	POINT	pos{ WINDOWSIZE_X / 2, WINDOWSIZE_Y - (WINDOWSIZE_Y / 3) }; // 임시 위치  

public:
	bool IsMoving() { return isMoving; }
	void Move();
	void SepSpeed(float speed);
	void Stop();
	void SetDir(Dir d);

};

class BattleScene : public Scene	
{
private:
	int curPlayerCnt = 0; // 현재 배틀씬에 접속된 플레이어 수 
	std::unordered_map<int, std::shared_ptr<BattlePlayer>> players{};

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<BattlePlayer>>& GetPlayers() const { return players; }

	void AddPlayer(int clientID);


public:
	void UpdatePlayer();
	void UpdateEnemy();
	void UpdateBoss();
	void UpdatePlayerSkill();
	void UpdateBossSkill();


};

