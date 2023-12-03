#pragma once
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Boss.h"

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
	PlayerBullet*		playerbullet;
	EnemyController*	enemies;
	Boss*				boss;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<Player>>& GetPlayers() const { return players; }
	Boss* GetBoss() { return boss; }
	const EnemyController* GetEnemyController() { return enemies; }
	const PlayerBullet* GetPlayerController() { return playerbullet; }

	void AddClient(int clientID);
	void RemoveClient(int clientID) {};



public:
	void ActiveSkill(int ID, std::shared_ptr<Player> player, Skill skill);
	void CollideCheck();

private:
	void CollideCheck_EnemyBullets_Player(int clientID,  Player* player);
	void CollideCheck_PlayerBullets_Enemies(int clientID,  Player* player);
	void CollideCheck_PlayerBullets_Boss(int clientID,  Player* player);

public:
	RECT GetRectDisplay() const
	{
		RECT rectDisplay = RECT{};
		return rectDisplay;
	}

};

