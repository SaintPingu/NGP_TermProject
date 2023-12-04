#pragma once
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Boss.h"

class StagePlayer;
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
	std::shared_ptr<EnemyController>	enemies{};
	std::shared_ptr<Boss>				boss{};
	std::shared_ptr<SkillManager>		skillManager{};

	float updateMPDelay = 0.f;
	bool firstUpdate{};

	void BattleEnd();
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void ProcessCommand(int clientID, Command command, void* data) override;

	const std::unordered_map<int, std::shared_ptr<Player>>& GetPlayers() const { return players; }
	std::shared_ptr<Boss> GetBoss() { return boss; }
	const std::shared_ptr<EnemyController> GetEnemyController() { return enemies; }

	const std::vector<BulletController::Bullet*>& GetEnemyBullets()						{ return enemies->GetEnemyBullets()->GetBullets(); }
	const std::vector<BulletController::Bullet*>& GetPlayerMainBullets(int clientID)	{ return players[clientID]->GetMainBullets()->GetBullets(); }
	const std::vector<BulletController::Bullet*>& GetPlayerSubBullets(int clientID)		{ return players[clientID]->GetSubBullets()->GetBullets(); }

	void AddClient(int clientID);	// not use
	void AddPlayer(const std::shared_ptr<StagePlayer>& p);
	void RemoveClient(int clientID);



public:
	void ActiveSkill(int ID, std::shared_ptr<Player> player, Skill skill);
	void CollideCheck();

private:
	void CollideCheck_Enemies_Player(Player* player);
	void CollideCheck_EnemyBullets_Player(Player* player);
	void CollideCheck_PlayerBullets_Enemies(Player* player);

public:
	RECT GetRectDisplay() const
	{
		RECT rectDisplay = RECT{};
		return rectDisplay;
	}

};

void BattleStart(const std::shared_ptr<StagePlayer>& p1, const std::shared_ptr<StagePlayer>& p2);