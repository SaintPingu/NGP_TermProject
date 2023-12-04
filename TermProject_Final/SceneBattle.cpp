#include "stdafx.h"
#include "SceneBattle.h"
#include "InputManager.h"
#include "Player.h"
#include "Enemy.h"
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
	for (auto& effect : effects) {
		effect.Render(hdc, effectImages[effect.type]);
	}
}

void SceneBattle::RenderEnemies(HDC hdc)
{
	for (auto& [id, enemy] : enemies) {
		switch (enemy.type) {
		case EnemyType::Melee:
			enemy.Render(hdc, meleeEnemy);
			break;
		case EnemyType::Range:
			enemy.Render(hdc, rangeEnemy);
			break;
		case EnemyType::Boss:
			boss->SetPos(enemy.pos);
			boss->Render(hdc);
			break;
		default:
			assert(0);
			break;
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

void SceneBattle::AnimateEffects()
{
	for (size_t i = 0; i < effects.size(); ++i)
	{
		if (effects.at(i).Animate() == false)
		{
			effects[i--] = effects.back();
			effects.pop_back();
		}
	}
}

void SceneBattle::CreateEffect(const Battle::EffectData& data)
{
	Effect effect{};
	effect.type = (EffectType)data.type;
	effect.pos = data.pos;
	effect.maxFrame = effectImages[effect.type].GetMaxFrame();

	effects.push_back(effect);
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

	// 스테이지별 적 로드
	stage = StageElement::Water; // 테스트 스테이지
	switch (stage) {
	case StageElement::Water:
		imgMelee.Load(L"images\\battle\\sprite_wingull.png", { 34,33 }, { 4,6 }, { 28,22 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load(L"images\\battle\\sprite_seadra.png", { 29,31 }, { 3,3 }, { 25,28 });
		imgRange.ScaleImage(1.2f, 1.2f);

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

	// 이펙트 이미지
	effectImages[EffectType::Explode_Fire].Load(_T("images\\battle\\explode_fire.png"), { 56, 56 }, 8);
	effectImages[EffectType::Explode_Fire].ScaleImage(.7f, .7f);
	effectImages[EffectType::Explode_Elec].Load(_T("images\\battle\\explode_elec.png"), { 81, 73 }, 8);
	effectImages[EffectType::Explode_Elec].ScaleImage(.6f, .6f);
	effectImages[EffectType::Explode_Water].Load(_T("images\\battle\\explode_water.png"), { 72, 64 }, 9);
	effectImages[EffectType::Explode_Water].ScaleImage(.7f, .7f);
	effectImages[EffectType::Explode_Dark].Load(_T("images\\battle\\explode_dark.png"), { 40, 40 }, 12);

	effectImages[EffectType::Cloud_Fire].Load(_T("images\\battle\\sprite_cloud_fire.png"), { 56, 64 }, 11);
	effectImages[EffectType::Cloud_Fire].ScaleImage(.8f, .8f);
	effectImages[EffectType::Cloud_Elec].Load(_T("images\\battle\\sprite_cloud_elec.png"), { 56, 64 }, 11);
	effectImages[EffectType::Cloud_Elec].ScaleImage(.8f, .8f);
	effectImages[EffectType::Cloud_Water].Load(_T("images\\battle\\sprite_cloud_water.png"), { 48, 56 }, 15);
	effectImages[EffectType::Cloud_Water].ScaleImage(.9f, .9f);
	effectImages[EffectType::Cloud_Dark].Load(_T("images\\battle\\sprite_cloud_dark.png"), { 56, 64 }, 11);

	// 적
	Vector2 pos{};
	meleeEnemy = std::make_shared<Melee>(imgMelee, pos);
	rangeEnemy = std::make_shared<Range>(imgRange, pos);

	// 보스 초기화
	boss = std::make_shared<Boss>();
	enemies[-1] = EnemyData();
	enemies[-1].type = EnemyType::Boss;
	enemies[-1].pos = { -9999, -9999 };

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
	//boss->Render(hdc);
	RenderEnemies(hdc);
	//player->RenderSkill(hdc);
	RenderEffects(hdc);
	gui->Render(hdc);
}

void SceneBattle::Animate()
{
	AnimatePlayers();
	if (boss) {
		boss->Animate(HWND());
	}
	AnimateEffects();

	gui->Update(HWND());
}


void SceneBattle::GetInput(CommandList* cmdList)
{
	if (cmdList == nullptr) {
		return;
	}
	cmdList->buffer.clear();

	if (KEY_TAP('Q')) {
		cmdList->PushCommand((BYTE)ClientBattleCmd::SkillQ, nullptr, 0);
	}
	if (KEY_TAP('W')) {
		cmdList->PushCommand((BYTE)ClientBattleCmd::SkillW, nullptr, 0);
	}
	if (KEY_TAP('E')) {
		cmdList->PushCommand((BYTE)ClientBattleCmd::SkillE, nullptr, 0);
	}

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

	ClientBattleCmd cmd;
	if (v != 0) {
		if (v == -1) {
			cmd = ClientBattleCmd::MoveLeft;
		}
		else {
			cmd = ClientBattleCmd::MoveRight;
		}
	}
	else if (h != 0) {
		if (h == -1) {
			cmd = ClientBattleCmd::MoveUp;
		}
		else {
			cmd = ClientBattleCmd::MoveDown;
		}
	}
	else {
		cmd = ClientBattleCmd::Stop;
	}
	cmdList->PushCommand((BYTE)cmd, nullptr, 0);
}

void SceneBattle::WriteData(void* data)
{
	PacketBuffer* buffer = static_cast<PacketBuffer*>(data);

	Battle::BattleData battleData;
	// PlayerBattleData
	memcpy(battleData.PlayerBattleData, buffer->data(), sizeof(Battle::PlayerBattleData) * 2);
	RemoveData(*buffer, sizeof(Battle::PlayerBattleData) * 2);
	for (int i = 0; i < 2; ++i) {
		int clientID = battleData.PlayerBattleData[i].PlayerID;
		Vector2 pos = battleData.PlayerBattleData[i].Pos;
		if (!players.count(clientID)) {
			players[clientID] = std::make_shared<Player>(otherPlayer.fly, otherPlayer.gnd);
		}

		players[clientID]->SetPos(pos);
	}

	// EnemyBattleData - Cnt
	memcpy(&battleData.EnemyData.EnemyCnt, buffer->data(), sizeof(Battle::EnemyBattleData::EnemyCnt));
	RemoveData(*buffer, sizeof(Battle::EnemyBattleData::EnemyCnt));
	BYTE enemyCnt = battleData.EnemyData.EnemyCnt;

	// EnemyBattleData - Data
	battleData.EnemyData.Enemies = new Battle::EnemyBattleData::Data[enemyCnt];

	std::set<int> IDs{};
	memcpy(battleData.EnemyData.Enemies, buffer->data(), sizeof(Battle::EnemyBattleData::Data) * enemyCnt);
	RemoveData(*buffer, sizeof(Battle::EnemyBattleData::Data) * enemyCnt);
	for (int i = 0; i < enemyCnt; ++i) {
		const auto& enemyData = battleData.EnemyData.Enemies[i];

		int id = enemyData.ID;
		IDs.insert(id);
		if (!enemies.count(id)) {
			enemies[id] = EnemyData();
		}

		std::bitset<8> byte(enemyData.TypeDirActPad);
		std::bitset<2> type(byte.to_string().substr(0, 2));
		std::bitset<3> dir(byte.to_string().substr(2, 3));
		std::bitset<1> action(byte.to_string().substr(5, 1));

		enemies[id].type = (EnemyType)type.to_ulong();
		enemies[id].dir = (Dir)dir.to_ulong();
		enemies[id].isAction = (bool)action.to_ulong();
		enemies[id].pos = (enemyData.Pos);
	}

	// 사망한 적 제거
	std::vector<int> idToDelete{};
	for (auto& [id, enemy] : enemies) {
		if (!IDs.count(id) && id != -1) {
			idToDelete.push_back(id);
		}
	}
	for (int id : idToDelete) {
		enemies.erase(id);
	}

	// BulletsBattleData - Cnt
	memcpy(&battleData.BulletData.BulletCnt, buffer->data(), sizeof(BYTE));
	RemoveData(*buffer, sizeof(BYTE));
	BYTE bulletCnt = battleData.BulletData.BulletCnt;

	// BulletsBattleData - Data
	battleData.BulletData.BulletsData = new Battle::BulletsBattleData::Data[bulletCnt];
	bullets.clear();
	bullets.resize(bulletCnt);

	memcpy(battleData.BulletData.BulletsData, buffer->data(), sizeof(Battle::BulletsBattleData::Data) * bulletCnt);
	RemoveData(*buffer, sizeof(Battle::BulletsBattleData::Data) * bulletCnt);
	for (int i = 0; i < bulletCnt; ++i) {
		Battle::BulletsBattleData::Data* bulletData = &battleData.BulletData.BulletsData[i];
		bullets[i].type = (BulletType)bulletData->bulletType;
		bullets[i].pos = bulletData->Pos;
		bullets[i].dir = bulletData->Dir;
	}
	delete[] battleData.EnemyData.Enemies;
	delete[] battleData.BulletData.BulletsData;
	return;

	// BossEffectData - Cnt
	memcpy(&battleData.BossEffectData.EffectCnt, buffer->data(), sizeof(BYTE));
	RemoveData(*buffer, sizeof(BYTE));

	// BossEffectData - Data
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
	PacketLoader packetLoader = framework->GetPacketLoader();

	std::shared_ptr<SceneBattle> scene;
	
	// 배틀은 명령이 여러개이므로 반복필요
	while(true) {
		BYTE cmd{};
		PacketBuffer cmdData;
		packetLoader.PopCommand(cmd, cmdData, SceneType::Battle);
		switch ((ServerBattleCmd)cmd)
		{
		case ServerBattleCmd::None:
			return true;
		case ServerBattleCmd::Loss:
			std::cout << "[CMD] Loss\n";
			SceneMgr->LoadScene(SceneType::Stage);

			SoundMgr->StopEffectSound();
			SoundMgr->PlayEffectSound(EffectSound::Loss);
			SoundMgr->StopBGMSound();

			break;
		case ServerBattleCmd::Win:
			std::cout << "[CMD] Win\n";
			SceneMgr->LoadScene(SceneType::Stage);

			SoundMgr->StopEffectSound();
			SoundMgr->StopBossSound();
			break;
		case ServerBattleCmd::AcceptSkillQ:
			std::cout << "[CMD] AcceptSkillQ\n";
			players[framework->client_ID]->ActiveSkill(Skill::Identity);
			break;
		case ServerBattleCmd::Hit: {
			float hp;
			memcpy(&hp, cmdData.data(), sizeof(float));
			players[framework->client_ID]->SetHp(hp);
			SoundMgr->PlayHitSound(HitSound::Auto);
			gui->DisplayHurtFrame();
			break;
		}
		case ServerBattleCmd::UpdateMP: {
			float mp;
			memcpy(&mp, cmdData.data(), sizeof(float));
			players[framework->client_ID]->SetMp(mp);
			break;
		}
		case ServerBattleCmd::CreateEffect: {
			Battle::EffectData effectData;
			memcpy(&effectData, cmdData.data(), sizeof(Battle::EffectData));
			CreateEffect(effectData);
			break;
		}
		case ServerBattleCmd::BossPos: {
			Vector2 pos;
			memcpy(&pos, cmdData.data(), sizeof(Vector2));
			enemies[-1].pos = pos;
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

void SceneBattle::EnemyData::Render(HDC hdc, std::shared_ptr<Enemy> enemy)
{
	enemy->SetPos(pos);
	enemy->SetDir(dir);
	enemy->Render(hdc);
}


void SceneBattle::Effect::Render(HDC hdc, EffectImage& image)
{
	const RECT rectImage = ISprite::GetRectImage(image, (int)frame);
	image.Render(hdc, pos, &rectImage);
}

bool SceneBattle::Effect::Animate()
{
	frame += DeltaTime() * 15;
	if (frame >= maxFrame) {
		return false;
	}

	return true;
}