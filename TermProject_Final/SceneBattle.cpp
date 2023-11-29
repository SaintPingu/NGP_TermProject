#include "stdafx.h"
#include "SceneBattle.h"
#include "InputManager.h"
#include "Player.h"


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

	// 테스트 플레이어 생성
	CreatePlayer(0, Type::Elec, Type::Fire);

	// 테스트 총알 생성
	bulletsData.BulletCnt = 3 + 3 + 2;
	bulletsData.BulletsData = new Battle::BulletsBattleData::Data[bulletsData.BulletCnt];
	bulletsData.BulletsData[0].bulletType = (BYTE)BulletType::Main_Elec;
	bulletsData.BulletsData[0].Pos = Vector2(50, 50);
	bulletsData.BulletsData[0].Dir = Vector2(.2f, .8f);

	bulletsData.BulletsData[1].bulletType = (BYTE)BulletType::Main_Fire;
	bulletsData.BulletsData[1].Pos = Vector2(100, 50);
	bulletsData.BulletsData[1].Dir = Vector2(.6f, .4f);

	bulletsData.BulletsData[2].bulletType = (BYTE)BulletType::Main_Water;
	bulletsData.BulletsData[2].Pos = Vector2(150, 50);
	bulletsData.BulletsData[2].Dir = Vector2(-.6f, .4f);

	bulletsData.BulletsData[3].bulletType = (BYTE)BulletType::Sub_Elec;
	bulletsData.BulletsData[3].Pos = Vector2(50, 100);
	bulletsData.BulletsData[3].Dir = Vector2(.6f, -.4f);

	bulletsData.BulletsData[4].bulletType = (BYTE)BulletType::Sub_Fire;
	bulletsData.BulletsData[4].Pos = Vector2(100, 100);
	bulletsData.BulletsData[4].Dir = Vector2(.9f, .1f);

	bulletsData.BulletsData[5].bulletType = (BYTE)BulletType::Sub_Water;
	bulletsData.BulletsData[5].Pos = Vector2(150, 100);
	bulletsData.BulletsData[5].Dir = Vector2(0, 1.f);

	bulletsData.BulletsData[6].bulletType = (BYTE)BulletType::Enemy;
	bulletsData.BulletsData[6].Pos = Vector2(200, 150);
	bulletsData.BulletsData[6].Dir = Vector2(1.f, 0);

	bulletsData.BulletsData[7].bulletType = (BYTE)BulletType::Boss;
	bulletsData.BulletsData[7].Pos = Vector2(300, 150);
	bulletsData.BulletsData[7].Dir = Vector2(1.f, 0);
}

void SceneBattle::Render(HDC hdc)
{
	battleMap.Render(hdc, stage);
	for (auto& [id, player] : players) {
		player->Render(hdc);
	}

	for (int i = 0; i < bulletsData.BulletCnt; ++i) {
		BulletType bulletType = (BulletType)bulletsData.BulletsData[i].bulletType;
		Vector2 pos = bulletsData.BulletsData[i].Pos;
		Vector2 dir = bulletsData.BulletsData[i].Dir;

		FRECT rectBody;
		POINT bulletSize = bulletImages[bulletType].GetBodySize();
		rectBody.left = (float)pos.x - ((float)bulletSize.x / 2);
		rectBody.right = rectBody.left + bulletSize.x;
		rectBody.top = (float)pos.y - ((float)bulletSize.y / 2);
		rectBody.bottom = rectBody.top + bulletSize.y;

		Vector2 vPoints[4];
		GetRotationPos(rectBody, dir, Vector2::Up(), vPoints);
		bulletImages[bulletType].RenderRotation(hdc, vPoints);
	}

	/*boss->Render(hdc);
	player->Render(hdc);
	enemies->Render(hdc);
	player->RenderSkill(hdc);
	effects->Render(hdc);
	gui->Render(hdc);*/
}

void SceneBattle::Animate()
{
	/*player->Animate();
	enemies->Animate();
	boss->AnimateSkill();
	boss->Animate();*/
}

void SceneBattle::GetInput(CommandList* cmdList)
{
	if (KEY_TAP('O')) {
		//player->Heal();
	}
	if(KEY_TAP('P')) {
		//player->FullMP();
	}
	if(KEY_TAP('I')) {
		//player->InvincibleMode();
	}
	if(KEY_TAP('Q')) {
		//player->ActiveSkill(Skill::Identity);
	}
	if(KEY_TAP('W')) {
		//player->ActiveSkill(Skill::Sector);
	}
	if(KEY_TAP('E')) {
		//player->ActiveSkill(Skill::Circle);
	}

	bool isMove = false;
	if (KEY_TAP(VK_LEFT))
	{
		//player->Stop(Dir::Right);
		//player->SetDirection(Dir::Left);
		isMove = true;
	}
	if (KEY_TAP(VK_RIGHT))
	{
		/*player->Stop(Dir::Left);
		player->SetDirection(Dir::Right);*/
		isMove = true;
	}
	if (KEY_TAP(VK_UP))
	{
		/*player->Stop(Dir::Down);
		player->SetDirection(Dir::Up);*/
		isMove = true;
	}
	if (KEY_TAP(VK_DOWN))
	{
		/*player->Stop(Dir::Up);
		player->SetDirection(Dir::Down);*/
		isMove = true;
	}
}

void SceneBattle::WriteData(void* data)
{
}

void SceneBattle::ProcessCommand()
{
}

void SceneBattle::CreatePlayer(int id, Type type, Type subType)
{
	players[id] = std::make_shared<Player>(type, subType);
}
