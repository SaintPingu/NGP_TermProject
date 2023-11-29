#include "stdafx.h"
#include "BattleScene.h"
#include "DataBase.h"


void BattleScene::Init()
{

}

void BattleScene::Update()
{
	UpdatePlayer();
	UpdateEnemy();
	UpdateBoss();
	UpdatePlayerSkill();
	UpdateBossSkill();
}

void BattleScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientBattleCmd  clientCmd = (ClientBattleCmd)command;

	switch (clientCmd)
	{
	case ClientBattleCmd::Terminate:
	{
		players.erase(clientID);
		curPlayerCnt -= 1;
	}
		break;
		/// +----------------------------------
		///				   T A P
		/// ----------------------------------+	
	case ClientBattleCmd::MoveLeftTap:
	{
		player->SetDirection(Dir::Left);
	}
		break;
	case ClientBattleCmd::MoveRightTap:
	{
		player->SetDirection(Dir::Right);
	}
		break;
	case ClientBattleCmd::MoveUpTap:
	{
		player->SetDirection(Dir::Up);
	}
		break;
	case ClientBattleCmd::MoveDownTap:
	{
		player->SetDirection(Dir::Down);
	}
		break;
		
		/// +----------------------------------
		///				A W A Y
		/// ----------------------------------+	
	case ClientBattleCmd::MoveLeftAway:
	case ClientBattleCmd::MoveRightAway:
	case ClientBattleCmd::MoveUpAway:
	case ClientBattleCmd::MoveDownAway:
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
		Skill_Q();
		break;
	case ClientBattleCmd::SkillW:
		Skill_W();
		break;
	case ClientBattleCmd::SkillE:
		Skill_E();
		break;
	}
}

void BattleScene::AddPlayer(int clientID)
{
	 players[clientID] = std::make_shared<Player>(); 

}

void BattleScene::UpdatePlayer()
{
	for (auto& [clientID, player] : players) {
		if(player->IsMove())
			player->Move();
	}
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

void BattleScene::UpdateEnemy()
{
	
}

void BattleScene::UpdateBoss()
{

}

void BattleScene::UpdatePlayerSkill()
{

}

void BattleScene::UpdateBossSkill()
{

}


