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
		player->SetDir(Dir::Left);
	}
		break;
	case ClientBattleCmd::MoveRightTap:
	{
		player->SetDir(Dir::Right);
	}
		break;
	case ClientBattleCmd::MoveUpTap:
	{
		player->SetDir(Dir::Up);
	}
		break;
	case ClientBattleCmd::MoveDownTap:
	{
		player->SetDir(Dir::Down);
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
		player->Stop();
	}
		break;

		/// +----------------------------------
		///				S T O P
		/// ----------------------------------+	
	case ClientBattleCmd::Stop:
	{
		player->Stop();
	}
		break;


		/// +----------------------------------
		///				S K I L L 
		/// ----------------------------------+	
	case ClientBattleCmd::SkillQ:

		break;
	case ClientBattleCmd::SkillW:

		break;
	case ClientBattleCmd::SkillE:

		break;
	}
}

void BattleScene::AddPlayer(int clientID)
{
	 players[clientID] = std::make_shared<BattlePlayer>(); 

}

void BattleScene::UpdatePlayer()
{
	for (auto& [clientID, player] : players) {
		if(player->IsMoving())
			player->Move();
	}
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



void BattlePlayer::Move()
{
	pos.x += DeltaTime() * speed * dirVector.x;
	pos.y += DeltaTime() * speed * dirVector.y;
}

void BattlePlayer::SepSpeed(float speed)
{
}

void BattlePlayer::Stop()
{
	isMoving = false;
}

void BattlePlayer::SetDir(Dir d)
{
	isMoving  = true;
	curDir    = d;

	switch (curDir)
	{
	case Dir::Left:
	{
		dirVector.x = -1;
		dirVector.y = 0;
	}
		break;
	case Dir::Right:
	{
		dirVector.x = 1;
		dirVector.y = 0;
	}
		break;
	case Dir::Up:
	{
		dirVector.x = 0;
		dirVector.y = -1;
	}
		break;
	case Dir::Down:
	{
		dirVector.x = 0;
		dirVector.y = 1;
	}
		break;
	case Dir::LD:
	{
		dirVector.x = -1;
		dirVector.y = 1;
	}
		break;
	case Dir::LU:
	{
		dirVector.x = -1;
		dirVector.y = -1;
	}
		break;
	case Dir::RD:
	{
		dirVector.x = 1;
		dirVector.y = 1;
	}
		break;
	case Dir::RU:
	{
		dirVector.x = 1;
		dirVector.y = -1;
	}
		break;
	}

}


