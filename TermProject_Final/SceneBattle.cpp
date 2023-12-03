#include "stdafx.h"
#include "SceneBattle.h"
#include "InputManager.h"
#include "Player.h"
#include "Framework.h"
#include "SceneManager.h"
#include "ClientNetwork.h"
#include "ClientNetMgr.h"

struct PlayerType {
	Type fly{};
	Type gnd{};
};
PlayerType otherPlayer{};
PlayerType myPlayer{};

BattleMap::BattleMap()
{
	_water.Load(L"images\\battle\\Water_spstage.bmp");
	_fire.Load(L"images\\battle\\Fire_spstage.bmp");
	_electronic.Load(L"images\\battle\\Electronic_spstage.bmp");
	_dark.Load(L"images\\battle\\Dark_spstage.bmp");
}

void BattleMap::Init()
{
	_rectImage = { 0, 0, 500, 1500 };
	_rectDraw = { 0, -750, 500, 750 };
	_rectDraw2 = { 0, -2250, 500, -750 };
	_shakeCount = 0;
	_shakeWindow = 0;
	_mapSpeed = 5;
}

void BattleMap::Render(HDC hdc, StageElement _select_index)
{
	switch (_select_index)
	{
	case StageElement::Water:
		_water.Draw(hdc, _rectDraw, _rectImage);
		_water.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Fire:
		_fire.Draw(hdc, _rectDraw, _rectImage);
		_fire.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Elec:
		_electronic.Draw(hdc, _rectDraw, _rectImage);
		_electronic.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Dark:
		_dark.Draw(hdc, _rectDraw, _rectImage);
		_dark.Draw(hdc, _rectDraw2, _rectImage);
		break;
	}

	if (_rectDraw.bottom >= 3000)
	{
		_rectDraw.top = -1500;
		_rectDraw.bottom = 0;
	}
	if (_rectDraw2.bottom >= 3000)
	{
		_rectDraw2.top = -1500;
		_rectDraw2.bottom = 0;
	}

	if (isShake == true)
	{
		constexpr int shakeAmount = 5;
		if (_shakeWindow % 5 == 0 && _rectDraw.left == 0)
		{
			_rectDraw.left -= 5;
			_rectDraw.right -= 5;
			_rectDraw2.left -= 5;
			_rectDraw2.right -= 5;
		}
		if (_shakeWindow % 5 == 4 && _rectDraw.left == -5)
		{
			_rectDraw.left += 5;
			_rectDraw.right += 5;
			_rectDraw2.left += 5;
			_rectDraw2.right += 5;
		}

		++_shakeWindow;
		if (--_shakeCount <= 0)
		{
			_shakeWindow = 0;
			isShake = false;
		}
	}
	else
	{
		_rectDraw.left = 0;
		_rectDraw.right = 500;
		_rectDraw2.left = 0;
		_rectDraw2.right = 500;
	}
}

void SceneBattle::RenderPlayers(HDC hdc)
{
	for (auto& [id, player] : players) {
		player->Render(hdc);
	}
}

void SceneBattle::RenderBullets(HDC hdc)
{
	for(auto& bullet : bullets) {
		FRECT rectBody;
		POINT bulletSize = bulletImages[bullet.type].GetBodySize();
		rectBody.left = (float)bullet.pos.x - ((float)bulletSize.x / 2);
		rectBody.right = rectBody.left + bulletSize.x;
		rectBody.top = (float)bullet.pos.y - ((float)bulletSize.y / 2);
		rectBody.bottom = rectBody.top + bulletSize.y;

		Vector2 vPoints[4];
		GetRotationPos(rectBody, bullet.dir, Vector2::Up(), vPoints);
		bulletImages[bullet.type].RenderRotation(hdc, vPoints);
	}

}

void SceneBattle::RenderEffects(HDC hdc)
{
}

void SceneBattle::RenderEnemies(HDC hdc)
{
	for (auto& enemy : enemies) {
		if (enemy.type == Enemytype::Melee) {
			meleeEnemy->SetPos(enemy.pos);
			meleeEnemy->Render(hdc);
		}
		else if (enemy.type == Enemytype::Range) {
			rangeEnemy->SetPos(enemy.pos);
			rangeEnemy->Render(hdc);
		}
	}
}

void SceneBattle::AnimatePlayers()
{
	HWND hwnd;
	for (auto& [id,player] : players) {
		player->Animate(hwnd);
	}
}

void SceneBattle::Init()
{
	// 맵 초기화
	battleMap.Init();

	// 총알 이미지 생성
	for (int i = 0; i < (int)BulletType::_count; ++i) {
		bulletImages[(BulletType)i] = ObjectImage();
	}
	// 플레이어(메인, 하늘) 총알 로드
	bulletImages[BulletType::Main_Elec].Load(_T("images\\battle\\bullet_elec_main.png"), { 5,16 });
	bulletImages[BulletType::Main_Fire].Load(_T("images\\battle\\bullet_fire.png"), { 11,16 });
	bulletImages[BulletType::Main_Fire].ScaleImage(0.9f, 0.9f);
	bulletImages[BulletType::Main_Water].Load(_T("images\\battle\\bullet_ice.png"), { 7,15 });
	bulletImages[BulletType::Main_Water].ScaleImage(0.9f, 0.9f);

	// 플레이어(서브, 지상) 총알 로드
	bulletImages[BulletType::Sub_Elec].Load(_T("images\\battle\\bullet_elec.png"), { 11,30 });
	bulletImages[BulletType::Sub_Elec].ScaleImage(0.7f, 0.7f);
	bulletImages[BulletType::Sub_Fire].Load(_T("images\\battle\\bullet_flame.png"), { 8,16 });
	bulletImages[BulletType::Sub_Fire].ScaleImage(0.7f, 0.7f);
	bulletImages[BulletType::Sub_Water].Load(_T("images\\battle\\bullet_water.png"), { 8,24 });
	bulletImages[BulletType::Sub_Water].ScaleImage(0.8f, 0.7f);

	// 스테이지별 적 총알 로드
	stage = StageElement::Water; // 테스트 스테이지
	switch (stage) {
	case StageElement::Water:
		bulletImages[BulletType::Enemy].Load(_T("images\\battle\\bullet_seadra.png"), { 14, 14 });
		bulletImages[BulletType::Enemy].ScaleImage(1.2f, 1.2f);
		bulletImages[BulletType::Boss].Load(_T("images\\battle\\bullet_boss_water.png"), { 400, 400 });
		bulletImages[BulletType::Boss].ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Fire:
		bulletImages[BulletType::Enemy].Load(_T("images\\battle\\bullet_latias.png"), { 14, 14 });
		bulletImages[BulletType::Enemy].ScaleImage(0.8f, 0.8f);
		bulletImages[BulletType::Boss].Load(_T("images\\battle\\bullet_boss_fire.png"), { 400, 400 });
		bulletImages[BulletType::Boss].ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Elec:
		bulletImages[BulletType::Enemy].Load(_T("images\\battle\\bullet_zapdos.png"), { 14, 14 });
		bulletImages[BulletType::Enemy].ScaleImage(0.9f, 0.9f);
		bulletImages[BulletType::Boss].Load(_T("images\\battle\\bullet_boss_elec.png"), { 400, 400 });
		bulletImages[BulletType::Boss].ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Dark:
		bulletImages[BulletType::Enemy].Load(_T("images\\battle\\bullet_aerodactyl.png"), { 10, 10 });
		bulletImages[BulletType::Enemy].ScaleImage(1.6f, 1.6f);
		bulletImages[BulletType::Boss].Load(_T("images\\battle\\bullet_boss_dark.png"), { 700, 700 });
		bulletImages[BulletType::Boss].ScaleImage(0.03f, 0.03f);
		break;
	default:
		assert(0);
		break;
	}

	EnemyDataStatus enemydata;

	imgMelee.Load(L"images\\battle\\sprite_wingull.png", { 34,33 }, { 4,6 }, { 28,22 });
	imgMelee.ScaleImage(1.2f, 1.2f);
	imgRange.Load(L"images\\battle\\sprite_seadra.png", { 29,31 }, { 3,3 }, { 25,28 });
	imgRange.ScaleImage(1.2f, 1.2f);
	enemydata.type = Type::Water;

	meleeEnemy = std::make_shared<Melee>(Melee(imgMelee, { 0.0f,0.0f }, enemydata));
	rangeEnemy = std::make_shared<Range>(Range(imgRange, { 0.0f,0.0f }, enemydata));
	
	// 테스트 용 데이터 넣기
	/*EnemyData data;
	data.dir = Dir::Down;
	data.isAction = true;
	data.pos = Vector2(100.0f, 100.0f);
	data.type = Enemytype::Melee;
	for (int i = 0; i < 10;++i) {
		for (int j = 0;j < 10;++j) {
			data.pos = Vector2(100.0f + i * 30, 100.0f + j * 20);
			if (rand() % 2) {
				data.type = Enemytype::Range;
			}
			else {
				data.type = Enemytype::Melee;

			}
			enemies.push_back(data);
		}
	}

	boss = std::make_shared<Boss>();
	boss->Create();
	boss->SetPos(Vector2(100.0f, 100.0f));*/

	// 테스트 플레이어 생성
	//CreatePlayer(0, Type::Elec, Type::Fire);

	gui = std::make_shared<GUIManager>();
	players[framework->client_ID] = std::make_shared<Player>(myPlayer.fly, myPlayer.gnd);
	gui->SetPlayer(players[framework->client_ID]);

	CommandList* cmdList = &CLIENT_NETWORK->GetPacketGenerator().cmdList;
	cmdList->PushCommand((BYTE)ClientBattleCmd::Stop, nullptr, 0);
	framework->DefaultPacketSend();
}

void SceneBattle::Render(HDC hdc)
{
	battleMap.Render(hdc, stage);
	RenderPlayers(hdc);
	RenderBullets(hdc);
	if (boss) {
		boss->Render(hdc);
	}
	/*boss->Render(hdc);
	player->Render(hdc);*/
	RenderEnemies(hdc);
	/*player->RenderSkill(hdc);
	effects->Render(hdc);*/
	gui->Render(hdc);
}

void SceneBattle::Animate()
{
	AnimatePlayers();
	if (boss) {
		boss->Animate(HWND());
	}
	/*player->Animate();
	enemies->Animate();
	boss->AnimateSkill();
	boss->Animate();*/

	gui->Update(HWND());
}


#define PLAYERMOVETEST //define시 플레이어 움직임을 싱글에서 테스트 해볼수 있음.

void SceneBattle::GetInput(CommandList* cmdList)
{
	if (cmdList == nullptr) {
		//return;
	}

	if (KEY_TAP('O')) {
		//player->Heal();
	}
	if (KEY_TAP('P')) {
		//player->FullMP();
	}
	if (KEY_TAP('I')) {
		//player->InvincibleMode();
	}
	if (KEY_TAP('Q')) {
		//player->ActiveSkill(Skill::Identity);
	}
	if (KEY_TAP('W')) {
		//player->ActiveSkill(Skill::Sector);
	}
	if (KEY_TAP('E')) {
		//player->ActiveSkill(Skill::Circle);
	}

	bool isMove = false;
	if (KEY_TAP(VK_UP)) {
		h -= 1;
	}
	if (KEY_TAP(VK_DOWN)) {
		h += 1;
	}
	if (KEY_TAP(VK_LEFT)) {
		v -= 1;
	}
	if (KEY_TAP(VK_RIGHT)) {
		v += 1;
	}

	if (KEY_AWAY(VK_UP)) {
		h += 1;
	}
	if (KEY_AWAY(VK_DOWN)) {
		h -= 1;
	}
	if (KEY_AWAY(VK_LEFT)) {
		v += 1;
	}
	if (KEY_AWAY(VK_RIGHT)) {
		v -= 1;
	}


#ifdef PLAYERMOVETEST
	// 플레이어 움직임 테스트
	Vector2 pos = players[framework->client_ID]->GetPosCenter();
	if (v != 0) {
		if (v == -1) {
			players[framework->client_ID]->SetPos(Vector2(pos.x - 10.0f, pos.y));
		}
		else {
			players[framework->client_ID]->SetPos(Vector2(pos.x + 10.0f, pos.y));
		}
	}
	pos = players[framework->client_ID]->GetPosCenter();
	if (h != 0) {
		if (h == -1) {
			if (isQuitDialog) {
			}
			else {
				players[framework->client_ID]->SetPos(Vector2(pos.x, pos.y - 10.0f));
			}
		}
		else {
			players[framework->client_ID]->SetPos(Vector2(pos.x, pos.y + 10.0f));
		}
	}
#else
	ClientBattleCmd cmd;
	if (v != 0) {
		if (v == -1) {
			cmd = ClientBattleCmd::MoveLeft;
			isQuitDialog = false;
		}
		else {
			cmd = ClientBattleCmd::MoveRight;
			isQuitDialog = false;
		}
	}
	if (h != 0) {
		if (h == -1) {
			if (isQuitDialog) {
				cmd = ClientBattleCmd::Stop;
			}
			else {
				cmd = ClientBattleCmd::MoveUp;
			}
		}
		else {
			cmd = ClientBattleCmd::MoveDown;
			isQuitDialog = false;
		}
	}
	else {
		cmd = ClientBattleCmd::Stop;
	}

	cmdList->PushCommand((BYTE)cmd, nullptr, 0);

	if (KEY_TAP(VK_ESCAPE) || (isQuitDialog && KEY_TAP(VK_RETURN))) {
		framework->Terminate();
	}
#endif PLAYERMOVETEST
}

void SceneBattle::WriteData(void* data)
{
	PacketBuffer* buffer = static_cast<PacketBuffer*>(data);

	Battle::BattleData battleData;

	// PlayerBattleData
	for (int i = 0; i < 2; ++i) {
		memcpy(&battleData.PlayerBattleData, buffer->data(), sizeof(Battle::PlayerBattleData));
		RemoveData(*buffer, sizeof(Battle::PlayerBattleData));

		int clientID = battleData.PlayerBattleData->PlayerID;
		Vector2 pos = battleData.PlayerBattleData->Pos;
		if (!players.count(clientID)) {
			players[clientID] = std::make_shared<Player>(otherPlayer.fly, otherPlayer.gnd);
		}

		players[clientID]->SetPos(pos);
	}

	return;
	// EnemyBattleData - Cnt
	memcpy(&battleData.EnemyData.EnemyCnt, buffer->data(), sizeof(BYTE));
	RemoveData(*buffer, sizeof(BYTE));

	// EnemyBattleData - Data
	battleData.EnemyData.Enemys = new Battle::EnemyBattleData::Data[battleData.EnemyData.EnemyCnt];
	enemies.clear();
	enemies.resize(battleData.EnemyData.EnemyCnt);

	for (int i = 0; i < battleData.EnemyData.EnemyCnt; ++i) {
		memcpy(&battleData.EnemyData.Enemys[i], buffer->data(), sizeof(Battle::EnemyBattleData::Data));
		RemoveData(*buffer, sizeof(Battle::EnemyBattleData::Data));

		Battle::EnemyBattleData::Data* enemyData = &battleData.EnemyData.Enemys[i];
		std::bitset<8> byte(enemyData->TypeDirActPad);
		std::bitset<2> type(byte.to_string().substr(0, 2));
		std::bitset<3> dir(byte.to_string().substr(2, 3));
		std::bitset<1> action(byte.to_string().substr(5, 1));

		enemies[i].type = (static_cast<Enemytype>(type.to_ulong()));
		enemies[i].dir = (static_cast<Dir>(dir.to_ulong()));
		enemies[i].isAction = (static_cast<bool>(action.to_ulong()));
		enemies[i].pos = (enemyData->Pos);

		/*enemies[i].SetType(static_cast<Enemytype>(type.to_ulong()));
		enemies[i].SetDir(static_cast<Dir>(dir.to_ulong()));
		enemies[i].SetIsAction(static_cast<bool>(action.to_ulong()));
		enemies[i].SetPos(enemyData->Pos);*/
	}

	// BulletsBattleData - Cnt
	memcpy(&battleData.BulletData.BulletCnt, buffer->data(), sizeof(BYTE));
	RemoveData(*buffer, sizeof(BYTE));

	// BulletsBattleData - Data
	battleData.BulletData.BulletsData = new Battle::BulletsBattleData::Data[battleData.BulletData.BulletCnt];
	bullets.clear();
	bullets.resize(battleData.BulletData.BulletCnt);

	for (int i = 0; i < battleData.EnemyData.EnemyCnt; ++i) {
		memcpy(&battleData.BulletData.BulletsData[i], buffer->data(), sizeof(Battle::BulletsBattleData::Data));
		RemoveData(*buffer, sizeof(Battle::BulletsBattleData::Data));

		Battle::BulletsBattleData::Data* bulletData = &battleData.BulletData.BulletsData[i];
		bullets[i].type = static_cast<BulletType>(bulletData->bulletType);
		bullets[i].pos = bulletData->Pos;
		bullets[i].dir = bulletData->Dir;
	}

	// BulletsBattleData - Cnt
	memcpy(&battleData.BossEffectData.EffectCnt, buffer->data(), sizeof(BYTE));
	RemoveData(*buffer, sizeof(BYTE));

	// BulletsBattleData - Data
	battleData.BossEffectData.Effects = new Battle::BossSkillBattleData::Data[battleData.BossEffectData.EffectCnt];
	bullets.clear();
	bullets.resize(battleData.BossEffectData.EffectCnt);

	for (int i = 0; i < battleData.BossEffectData.EffectCnt; ++i) {
		memcpy(&battleData.BossEffectData.Effects[i], buffer->data(), sizeof(Battle::BulletsBattleData::Data));
		RemoveData(*buffer, sizeof(Battle::BulletsBattleData::Data));

		Battle::BossSkillBattleData::Data* effectData = &battleData.BossEffectData.Effects[i];
		effects[i].type = static_cast<EffectType>(effectData->type);
		effects[i].pos = effectData->pos;
	}
}

bool SceneBattle::ProcessCommand()
{
	BYTE cmd;
	PacketBuffer buffer;
	PacketLoader packetLoader = framework->GetPacketLoader();

	std::shared_ptr<SceneBattle> scene;
	
	// 배틀은 명령이 여러개이므로 반복필요
	while (packetLoader.PopCommand(cmd, buffer, SceneType::Battle)) {
		switch ((ServerBattleCmd)cmd)
		{
		case ServerBattleCmd::None:
			break;
		case ServerBattleCmd::Loss:
			std::cout << "[CMD] Loss\n";
			SceneMgr->LoadScene(SceneType::Stage);

			/*soundManager->StopEffectSound();
			soundManager->PlayEffectSound(EffectSound::Loss);
			soundManager->StopBGMSound();*/

			break;
		case ServerBattleCmd::Win:
			std::cout << "[CMD] Win\n";
			SceneMgr->LoadScene(SceneType::Stage);

			//soundManager->StopEffectSound();
			//soundManager->StopBossSound();
			break;
		case ServerBattleCmd::AcceptSkillQ:
			std::cout << "[CMD] AcceptSkillQ\n";
			players[framework->client_ID]->ActiveSkill(Skill::Identity);
			break;
		case ServerBattleCmd::Hit: {
			std::cout << "[CMD] Hit\n";
			float hp;
			memcpy(&hp, &(*buffer.begin()), sizeof(float));
			players[framework->client_ID]->SetHp(hp);
			break;
		}
		case ServerBattleCmd::UpdateMP: {
			std::cout << "[CMD] UpdateMP\n";
			float mp;
			memcpy(&mp, &(*buffer.begin()), sizeof(float));
			players[framework->client_ID]->SetMp(mp);
			break;
		}
		case ServerBattleCmd::CreateEffect: {
			std::cout << "[CMD] CreateEffect\n";
			Effect createEffect;
			memcpy(&createEffect.type, &(*buffer.begin()), sizeof(BYTE));
			buffer.erase(buffer.begin());
			memcpy(&createEffect.pos, &(*buffer.begin()), sizeof(float) * 2);
			effects.push_back(createEffect);
			break;
		}
		default:
			assert(0);
			break;
		}
	}

	return true;
}

void SceneBattle::CreatePlayer(int id, Type type, Type subType)
{
	players[id] = std::make_shared<Player>(type, subType);
}

void SetMyPlayer(Type fly, Type gnd)
{
	myPlayer.fly = fly;
	myPlayer.gnd = gnd;
}
void SetOtherPlayer(Type fly, Type gnd)
{
	otherPlayer.fly = fly;
	otherPlayer.gnd = gnd;
}
Type GetPlayerFlyType()
{
	return myPlayer.fly;
}
Type GetPlayerGndType()
{
	return myPlayer.gnd;
}