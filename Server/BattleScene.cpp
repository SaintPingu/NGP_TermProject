#include "stdafx.h"
#include "BattleScene.h"
#include "DataBase.h"

#include "ClientInfo.h"
#include "ClientMgr.h"
#include "ServerFramework.h"
#include "SceneMgr.h"
#include "Bullet.h"

bool isBattleStarted{};
void BattleStart()
{
	isBattleStarted = true;
	SCENE_MGR->GetGameData().isBattleStart = true;
	SCENE_MGR->Battle()->Init();
}

void BattleScene::Init()
{
	enemies = std::make_shared<EnemyController>();
}

void BattleScene::Update()
{
	if (!isBattleStarted) {
		return;
	}

	for (auto& [clientID, player] : players) {
		player->Move();
		player->CheckShot();
	}

	enemies->CreateCheckMelee();
	enemies->CreateCheckRange();
	enemies->CheckAttackDelay();
	enemies->Move();
	return;
	enemies->MoveBullets();

	for (auto& [clientID, player] : players) {
		player->MoveBullets();
	}


	if (boss) {
		for (auto& [clientID, player] : players) {
			boss->CheckActDelay(player.get());
		}
		boss->CheckAttackDelay();
		boss->Move();
	}


	// player->Animate(hWnd);
	// enemies->Animate();
	// boss->Animate();
	//boss->AnimateSkill();

}

void BattleScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientBattleCmd  clientCmd = (ClientBattleCmd)command;

	switch (clientCmd)
	{
	/// +----------------------------------
	///				 M O V E 
	/// ----------------------------------+	
	case ClientBattleCmd::MoveLeft:
	{
		player->SetDirection(Dir::Left);
	}
		break;
	case ClientBattleCmd::MoveRight:
	{
		player->SetDirection(Dir::Right);
	}
		break;
	case ClientBattleCmd::MoveUp:
	{
		player->SetDirection(Dir::Up);
	}
		break;
	case ClientBattleCmd::MoveDown:
	{
		player->SetDirection(Dir::Down);
	}
		break;
	case ClientBattleCmd::Stop:
	{
		player->SetDirection(Dir::Empty);
	}
		break;
	/// +----------------------------------
	///				S K I L L 
	/// ----------------------------------+	
	case ClientBattleCmd::SkillQ:
	{
		ActiveSkill(clientID, player, Skill::Identity);
	}
		break;
	case ClientBattleCmd::SkillW:
	{
		ActiveSkill(clientID, player, Skill::Sector); 
	}
		break;
	case ClientBattleCmd::SkillE:
	{
		ActiveSkill(clientID, player, Skill::Circle); 
	}
		break;
	}

}

void BattleScene::AddClient(int clientID)
{
	 players[clientID] = std::make_shared<Player>(); 
	 if (players.size() == 1) {
		 players[clientID]->SetPos({ 175, 500 });
	 }
	 else {
		 players[clientID]->SetPos({ 325, 500 });
	 }

}

void BattleScene::ActiveSkill(int ID, std::shared_ptr<Player> player, Skill skill)
{
	if (player->IsDeath())
		return;
	player->ActiveSkill(skill);

	switch (skill)
	{
	case Skill::Identity:
	{
		int RedMP = 30;
		if (player->ReduceMP(RedMP))
		{
			float MP = player->GetMP();
			CLIENT_MGR->PushCommand(ID, (BYTE)ServerBattleCmd::AcceptSkillQ, nullptr, 0);
			CLIENT_MGR->PushCommand(ID, (BYTE)ServerBattleCmd::UpdateMP, (PVOID)&MP, sizeof(float));
		}
	}
		break;
	case Skill::Sector:
	{
		int RedMP = 15;
		if (player->ReduceMP(RedMP))
		{
			float MP = player->GetMP();
			CLIENT_MGR->PushCommand(ID, (BYTE)ServerBattleCmd::UpdateMP, (PVOID)&MP, sizeof(float));
		}
	}
		break;
	case Skill::Circle:
	{
		int RedMP = 10;
		if (player->ReduceMP(RedMP))
		{
			float MP = player->GetMP();
			CLIENT_MGR->PushCommand(ID, (BYTE)ServerBattleCmd::UpdateMP, (PVOID)&MP, sizeof(float));
		}
	}
		break;
	}

}

void BattleScene::CollideCheck()
{
	for (auto& [clientID, player] : players) {
		
	/// +----------------------------------
	///	     ENEMY BULLETS <-> PLAYER 
	/// ----------------------------------+	
		CollideCheck_EnemyBullets_Player(clientID, player.get());

	/// +----------------------------------
	///	  PLAYER BULLETS <-> ENEMIES, BOSS
	/// ----------------------------------+	
		CollideCheck_PlayerBullets_Enemies(clientID, player.get());
	}


}

void BattleScene::CollideCheck_EnemyBullets_Player(int clientID,  Player* player)
{
	const std::vector<BulletController::Bullet*> enemyBullets = enemies->GetEnemyBullets()->GetBullets();
	for (size_t i = 0; i < enemyBullets.size(); ++i)
	{
		if (player->IsCollide(enemyBullets.at(i)->GetRect()) == true)
		{
			player->Hit(enemyBullets.at(i)->GetDamage()
				, enemyBullets.at(i)->GetType()
				, enemyBullets.at(i)->GetPos());

			enemies->GetEnemyBullets()->Pop(i);

		}
		else if (enemyBullets.at(i)->Move() == false)
		{
			enemies->GetEnemyBullets()->Pop(i);

		}
	}
}

void BattleScene::CollideCheck_PlayerBullets_Enemies(int clientID,  Player* player)
{
	const std::vector<BulletController::Bullet*> playerBullets    = player->GetPlayerBullets()->GetBullets();
	const std::vector<BulletController::Bullet*> playerSubBullets = player->GetPlayerSubBullets()->GetBullets();


	for (size_t i = 0; i < playerBullets.size(); ++i)
	{
		const RECT	rectBullet    = playerBullets.at(i)->GetRect();
		const float bulletDamage  = playerBullets.at(i)->GetDamage();
		const Type	bulletType    = playerBullets.at(i)->GetType();
		const POINT bulletPos     = playerBullets.at(i)->GetPos();
		
		if ((enemies->CheckHit(rectBullet, bulletDamage, bulletType, bulletPos) == true) ||
			(boss->CheckHit(rectBullet, bulletDamage, bulletType, bulletPos) == true))
		{
			if (playerBullets.at(i)->IsSkillBullet() == false)
			{
				player->AddMP(0.15f);
				float MP = player->GetMP();
				CLIENT_MGR->PushCommand(clientID, (BYTE)ServerBattleCmd::UpdateMP, (PVOID)&MP, sizeof(float));

			}
			player->GetPlayerBullets()->Pop(i);
		}
		else if (playerBullets.at(i)->Move() == false)
		{
			player->GetPlayerBullets()->Pop(i);
		}
	}
}

