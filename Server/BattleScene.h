#pragma once
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Boss.h"
//#include "Battle.h"


const int maxBlattlePlayer = 2;
class BattleScene : public Scene	
{
private:
	/// +----------------------------------
	///				  PLAYER 
	/// ----------------------------------+	
	int curPlayerCnt = 0; // 현재 배틀씬에 접속된 플레이어 수 
	std::unordered_map<int, std::shared_ptr<Player>> players{};
		
	/// +----------------------------------
	///				  ENEMY  
	/// ----------------------------------+	
	EnemyController*	enemies;
	EnemyBullet*		enemyBullets;
	Boss*				boss;
	//BattleInfo*			battle;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<Player>>& GetPlayers() const { return players; }
	Boss* GetBoss() { return boss; }
	//BattleInfo* GetBattle() { return battle; }

	void AddPlayer(int clientID);


public:
	void UpdatePlayer();
	void UpdateEnemy();
	void UpdateBoss(Player* player);
	void UpdatePlayerSkill();
	void UpdateBossSkill();

	void Skill_Q();
	void Skill_W();
	void Skill_E();

	RECT GetRectDisplay() const
	{
		RECT rectDisplay = RECT{};
		return rectDisplay;
	}

};

