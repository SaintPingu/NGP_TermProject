#include "stdafx.h"
#include "BattleScene.h"
#include "DataBase.h"

#include "ClientInfo.h"
#include "ClientMgr.h"
#include "ServerFramework.h"
#include "SceneMgr.h"
#include "Bullet.h"
#include "StageScene.h"

bool isBattleStarted{};

void BattleStart(const std::shared_ptr<StagePlayer>& p1, const std::shared_ptr<StagePlayer>& p2)
{
	isBattleStarted = true;
	SCENE_MGR->GetGameData().isBattleStart = true;

	auto& battle = SCENE_MGR->Battle();
	battle->Init();
	battle->AddPlayer(p1);
	battle->AddPlayer(p2);
}

void BattleScene::BattleEnd()
{
	isBattleStarted = false;
	SCENE_MGR->GetGameData().isBattleStart = false;

	// 메모리 해제
	enemies = nullptr;
	boss = nullptr;
	skillManager = nullptr;
	players.clear();
}

void BattleScene::Init()
{
	enemies = std::make_shared<EnemyController>();
	boss = std::make_shared<Boss>();
	skillManager = std::make_shared<SkillManager>();
	firstUpdate = true;
	elapsedTime = 0.f;
}

void BattleScene::Update()
{
	//constexpr int fieldTime = 36;
	constexpr int fieldTime = 1;

	if (!isBattleStarted) {
		return;
	}
	if (firstUpdate) {
		for (auto& [clientID, player] : players) {
			float mp = player->GetMP();
			CLIENT_MGR->PushCommand(clientID, (BYTE)ServerBattleCmd::UpdateMP, &mp, sizeof(float));
		}
	}

	int deathCount{};
	for (auto& [clientID, player] : players) {
		if (player->IsDeath()) {
			++deathCount;
			continue;
		}
		player->Move();
		player->CheckShot();
	}
	if (deathCount == 2) {
		BattleEnd();
		return;
	}

	elapsedTime += DeltaTime();
	if (elapsedTime >= fieldTime) {
		boss->Create();
	}

	constexpr int MPUpdateDelay = 1.f;
	constexpr int MPIncAmount = 3.f;
	updateMPDelay += DeltaTime();
	if (updateMPDelay >= MPUpdateDelay) {
		updateMPDelay = 0;
		for (auto& [clientID, player] : players) {
			player->AddMP(MPIncAmount);
			float mp = player->GetMP();
			CLIENT_MGR->PushCommand(clientID, (BYTE)ServerBattleCmd::UpdateMP, &mp, sizeof(float));
		}
	}

	if (!boss->IsCreated())
	{
		enemies->CreateCheckMelee();
		enemies->CreateCheckRange();
	}
	else {
		Vector2 pos = boss->GetPosCenter();
		for (auto& [clientID, player] : players) {
			CLIENT_MGR->PushCommand(clientID, (BYTE)ServerBattleCmd::BossPos, &pos, sizeof(Vector2));
		}
	}
	enemies->CheckAttackDelay();
	enemies->Move();
	enemies->MoveBullets();

	for (auto& [clientID, player] : players) {
		player->MoveBullets();
	}
	CollideCheck(); // 충돌체크 

	if (boss) {
		//for (auto& [clientID, player] : players) {
		//	boss->CheckActDelay(player.get());
		//}
		//boss->CheckAttackDelay();
		boss->Move();
	}


	// player->Animate(hWnd);
	// enemies->Animate();
	// boss->Animate();
	//boss->AnimateSkill();



}

void BattleScene::ProcessCommand(int clientID, Command command, void* data)
{
	if (!players.count(clientID)) {
		return;
	}

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
	// not use
}

void BattleScene::RemoveClient(int clientID)
{
	// 다른 클라이언트에게 종료 메세지를 보내고 씬을 종료한다.
	for (auto& [playerID, player] : players) {
		CLIENT_MGR->PushCommand(playerID, (BYTE)ServerBattleCmd::Loss, nullptr, 0);
	}

	BattleEnd();
}

void BattleScene::AddPlayer(const std::shared_ptr<StagePlayer>& p)
{
	auto& player = players[p->ID];
	player = std::make_shared<Player>(p->typeFly, p->typeGnd);
	player->Init(p->ID);
	if (players.size() == 1) {
		player->SetPos({ 175, 500 });
	}
	else {
		player->SetPos({ 325, 500 });
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
			player->ActiveSkill(Skill::Sector);
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
			player->ActiveSkill(Skill::Circle);
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
		CollideCheck_EnemyBullets_Player(player.get());

	/// +----------------------------------
	///	  PLAYER BULLETS <-> ENEMIES, BOSS
	/// ----------------------------------+	
		CollideCheck_PlayerBullets_Enemies(player.get());
	}


}

void BattleScene::CollideCheck_EnemyBullets_Player(Player* player)
{
	auto& enemyBullets = enemies->GetEnemyBullets();
	for (size_t i = 0; i < enemyBullets->GetBulletCount(); ++i)
	{
		auto bullet = enemyBullets->GetBullet(i);
		if (player->IsCollide(bullet->GetRect()) == true)
		{
			player->Hit(bullet->GetDamage(), bullet->GetType(), bullet->GetPos());

			enemyBullets->Pop(i);
		}
	}
}

void BattleScene::CollideCheck_PlayerBullets_Enemies(Player* player)
{
	auto& mainBullets	= player->GetMainBullets();
	auto& subBullets	= player->GetSubBullets();

	for (size_t b = 0; b < 2; ++b) {
		std::shared_ptr<PlayerBullet> bullets{};

		switch (b) {
		case 0:
			bullets = mainBullets;
			break;
		case 1:
			bullets = subBullets;
			break;
		default:
			assert(0);
			break;
		}

		for (size_t i = 0; i < bullets->GetBulletCount(); ++i)
		{
			auto bullet = bullets->GetBullet(i);
			const RECT	rectBullet = bullet->GetRect();
			const float bulletDamage = bullet->GetDamage();
			const Type	bulletType = bullet->GetType();
			const POINT bulletPos = bullet->GetPos();

			if ((enemies->CheckHit(rectBullet, bulletDamage, bulletType, bulletPos) == true) ||
				(boss->CheckHit(rectBullet, bulletDamage, bulletType, bulletPos) == true))
			{
				bullets->Pop(i);
			}
		}
	}
}



