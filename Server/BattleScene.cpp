#include "stdafx.h"
#include "BattleScene.h"
#include "DataBase.h"


void BattleScene::Init()
{

}

void BattleScene::Update()
{
	//UpdatePlayer();
	//UpdateEnemy();
	//UpdateBoss();
	//UpdatePlayerSkill();
	//UpdateBossSkill();
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

}

void BattleScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientBattleCmd  clientCmd = (ClientBattleCmd)command;

	switch (clientCmd)
	{
	//case ClientBattleCmd::Terminate:
	//{
	//	players.erase(clientID);
	//	curPlayerCnt -= 1;
	//}
	//	break;
	case ClientBattleCmd::MoveLeft:
	{

	}
	break;
		/// +----------------------------------
		///				   T A P
		/// ----------------------------------+	
	//case ClientBattleCmd::MoveLeftTap:
	{
		player->SetDirection(Dir::Left);
	}
		break;
	//case ClientBattleCmd::MoveRightTap:
	{
		player->SetDirection(Dir::Right);
	}
		break;
	//case ClientBattleCmd::MoveUpTap:
	{
		player->SetDirection(Dir::Up);
	}
		break;
	//case ClientBattleCmd::MoveDownTap:
	{
		player->SetDirection(Dir::Down);
	}
		break;
		
		/// +----------------------------------
		///				A W A Y
		/// ----------------------------------+	
	//case ClientBattleCmd::MoveLeftAway:
	//case ClientBattleCmd::MoveRightAway:
	//case ClientBattleCmd::MoveUpAway:
	//case ClientBattleCmd::MoveDownAway:
	{
		player->StopMove();
	}
		break;

		/// +----------------------------------
		///				S T O P
		/// ----------------------------------+	
	case ClientBattleCmd::Stop:
	{
		player->StopMove();
	}
		break;


		/// +----------------------------------
		///				S K I L L 
		/// ----------------------------------+	
	case ClientBattleCmd::SkillQ:
		player->ActiveSkill(Skill::Identity);
		/*
			AcceptSkillQ
			-> SEver가 클라한테 클라가 Q 를 쓸수있는지를 판단한다. 

		*/
		Skill_Q();
		break;
	case ClientBattleCmd::SkillW:
		player->ActiveSkill(Skill::Sector);

		Skill_W();
		break;
	case ClientBattleCmd::SkillE:
		player->ActiveSkill(Skill::Circle);

		Skill_E();
		break;
	}
}

void BattleScene::AddClient(int clientID)
{
	 players[clientID] = std::make_shared<Player>(); 

}

void BattleScene::UpdatePlayer()
{
	for (auto& [clientID, player] : players) {
		if(player->IsMove())
			player->Move();

		player->CheckShot();

	}
}

void BattleScene::UpdateEnemy()
{
	enemies->CreateCheckMelee();
	enemies->CreateCheckRange();
	enemies->CheckAttackDelay();

}

void BattleScene::UpdateBoss(Player* player)
{
	boss->CheckActDelay(player);
	boss->CheckAttackDelay();

}

void BattleScene::UpdatePlayerSkill()
{

}

void BattleScene::UpdateBossSkill()
{

}


void BattleScene::Skill_Q()
{
	
}

void BattleScene::Skill_W()
{

}

void BattleScene::Skill_E()
{

}


