#include "stdafx.h"
#include "BattleScene.h"
#include "DataBase.h"

#include "ClientInfo.h"
#include "ClientMgr.h"
#include "Bullet.h"


void BattleScene::Init()
{

}

void BattleScene::Update()
{
	if (!isBattleStarted) {
		return;
	}

	for (auto& [clientID, player] : players) {
		if (player->IsMove())
			player->Move();
		player->CheckShot();
	}
	enemies->CreateCheckMelee();
	enemies->CreateCheckRange();
	enemies->CheckAttackDelay();

	for (auto& [clientID, player] : players) {
		boss->CheckActDelay(player.get());

	}
	boss->CheckAttackDelay();

	for (auto& [clientID, player] : players) {
		player->MoveBullets();
	}
	enemies->MoveBullets();
	enemies->Move();
	boss->Move();

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
		player->Stop(Dir::Right);
		player->SetDirection(Dir::Left);
		player->StartMove();
	}
		break;
	case ClientBattleCmd::MoveRight:
	{
		player->Stop(Dir::Left);
		player->SetDirection(Dir::Right);
		player->StartMove();
	}
		break;
	case ClientBattleCmd::MoveUp:
	{
		player->Stop(Dir::Down);
		player->SetDirection(Dir::Up);
		player->StartMove();
	}
		break;
	case ClientBattleCmd::MoveDown:
	{
		player->Stop(Dir::Up);
		player->SetDirection(Dir::Down);
		player->StartMove();
	}
		break;
	case ClientBattleCmd::Stop:
	{
		player->StopMove();
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

