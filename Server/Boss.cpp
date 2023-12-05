#include "stdafx.h"
#include "Boss.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "skill.h"
#include "scene.h"
#include "Image.h"
#include "SceneMgr.h"
#include "ServerFramework.h"
#include "BattleScene.h"
#include "ServerPacket.h"



void Boss::SetMove(const Vector2& unitVector)
{
	this->unitVector = unitVector;
	StartMove();
}
void Boss::Death()
{
	bossData.hp = 0;
	bossData.isDeath = true;

	
	std::unordered_map<int, std::shared_ptr<Player>> players = SCENE_MGR->Battle()->GetPlayers();
	for (auto& iter : players)
	{
		iter.second.get()->InvincibleMode();
	}
	//player->InvincibleMode();

	//soundManager->StopEffectSound();
	//soundManager->StopBossSound();
}
void Boss::Shot()
{
	act = static_cast<BossAct>(rand() % 5);
	switch (act)
	{
	case BossAct::Line:
		ShotByLine();
		break;
	case BossAct::Sector:
		ShotBySector();
		break;
	case BossAct::Circle:
		ShotByCircle();
		break;
	case BossAct::Spiral:
		ShotBySector();
		break;
	case BossAct::Spread:
		ShotBySector();
		break;
	default:
		return;
	}
	skillCount = 1;
}
BulletData Boss::GetBulletData()
{
	BulletData bulletData;

	bulletData.bulletType = bossData.type;
	bulletData.damage = bossData.damage;
	bulletData.speed = bossData.bulletSpeed[static_cast<int>(act)];

	return bulletData;
}
void Boss::ResetAttackDelay()
{
	bossData.crntAttackDelay = 10.f;
}


Boss::Boss()
{
	//const RECT rectDisplay = sceneManager->Battle()->GetRectDisplay();

	ObjectImage imgBullet;
	image = new ObjectImage();

	switch (SCENE_MGR->GetCrntStageType())
	{
	case StageElement::Elec:
		image->Load({ 73,68 }, { 69,50 });
		image->ScaleImage(4,4);
		imgBullet.Load({400,400});
		imgBullet.ScaleImage(0.05f, 0.05f);
		//image->Load(_T("images\\battle\\sprite_boss_elec.png"), { 73,68 }, { 3,7 }, { 69,50 });
		//image->ScaleImage(4, 4);
		//imgBullet.Load(_T("images\\battle\\bullet_boss_elec.png"), { 400,400 });
		//imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Water:
		image->Load({ 65,41 }, { 63,36 });
		image->ScaleImage(4, 4);
		imgBullet.Load({ 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);

		//image->Load(_T("images\\battle\\sprite_boss_water.png"), { 65,41 }, { 2,3 }, { 63,36 });
		//image->ScaleImage(4, 4);
		//imgBullet.Load(_T("images\\battle\\bullet_boss_water.png"), { 400,400 });
		//imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Fire:
		image->Load({ 54,44 }, { 44,29 });
		image->ScaleImage(6,6);
		imgBullet.Load({ 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);

		//image->Load(_T("images\\battle\\sprite_boss_fire.png"), { 54,44 }, { 6,12 }, { 44,29 });
		//image->ScaleImage(6, 6);
		//imgBullet.Load(_T("images\\battle\\bullet_boss_fire.png"), { 400,400 });
		//imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Dark:
		image->Load({ 110,110 }, { 67,50 });
		image->ScaleImage(5,5);
		imgBullet.Load({ 700,700 });
		imgBullet.ScaleImage(0.03f, 0.03f);

		//image->Load(_T("images\\battle\\sprite_boss_dark.png"), { 110,110 }, { 20,30 }, { 67,50 });
		//image->ScaleImage(5, 5);
		//imgBullet.Load(_T("images\\battle\\bullet_boss_dark.png"), { 700,700 });
		//imgBullet.ScaleImage(0.03f, 0.03f);
		break;
	default:
		assert(0);
		break;
	}

	bullets = std::make_shared<EnemyBullet>(imgBullet);
	maxSkillCount[static_cast<unsigned int>(BossAct::Line)] = INT_MAX;
	maxSkillCount[static_cast<unsigned int>(BossAct::Circle)] = 10;
	maxSkillCount[static_cast<unsigned int>(BossAct::Spiral)] = 360;
	maxSkillCount[static_cast<unsigned int>(BossAct::Sector)] = 15;
	maxSkillCount[static_cast<unsigned int>(BossAct::Spread)] = 720;
}
Boss::~Boss()
{
	delete image;
}

void Boss::Create()
{
	if (bossData.isCreated) {
		return;
	}

	bossData = GetBossData();
	bossData.isCreated = true;
	skill = std::make_shared<BossSkillManager>();

	Vector2 posCenter = { WINDOWSIZE_X / 2 , -300 };
	GameObject::Init(*image, posCenter);
	SetMove(Vector2::Down());
}
void Boss::SetPosDest()
{
	if (IsMove() == false)
	{
		return;
	}

	posDest = Vector2::GetDest(GetPosCenter(), unitVector, bossData.speed);
	const RECT rectBody = GetRectBody();
	const int maxYPos = (rectBody.bottom - rectBody.top) / 2;

	Vector2 unitVector = Vector2::Down();

	if (posDest.y > maxYPos)
	{
		StopMove();
		bossData.speed = 5;
	}
}

//void Boss::Paint(HDC hdc)
//{
//	constexpr int disappearFrame = -3;
//	if (bossData.isCreated == false)
//	{
//		return;
//	}
//	else if (deathFrame < disappearFrame)
//	{
//		return;
//	}
//
//	const RECT rectImage = ISprite::GetRectImage(GetImage(), frame);
//	GameObject::Paint(hdc, &rectImage);
//
//	bullets->Paint(hdc);
//	skill->Paint(hdc);
//
//	if (isShowHP)
//	{
//		SetBkMode(hdc, TRANSPARENT);
//		SetTextColor(hdc, RGB(255, 255, 255));
//
//		TCHAR txtHP[50];
//		wsprintf(txtHP, _T("BOSS HP : %d / %d"), (int)bossData.hp, (int)bossData.maxHP);
//		TextOut(hdc, 10, 10, txtHP, lstrlen(txtHP));
//	}
//}

void Boss::Move()
{
	if (bossData.isCreated == false)
	{
		return;
	}

	bullets->Move();

	if (bossData.isDeath == true)
	{
		return;
	}

	const RECT rectBody = GetRectBody();

	std::unordered_map<int, std::shared_ptr<Player>> players = SCENE_MGR->Battle()->GetPlayers();
	for (auto& iter : players)
	{
		if (iter.second.get()->IsCollide(rectBody) == true)
		{
			iter.second.get()->Hit(bossData.damage, GetType());
		}
	}

	if (IsMove() == false)
	{
		return;
	}

	SetPosDest();
	SetPos(posDest);
}
void Boss::CheckAttackDelay()
{
	if (bossData.isCreated == false)
	{
		return;
	}
	else if (bossData.isDeath == true)
	{
		return;
	}

	bossData.crntAttackDelay -= DeltaTime();
	if (bossData.crntAttackDelay <= 0.f)
	{
		bossData.crntAttackDelay = 1.5f;
		Shot();
	}
}

bool Boss::CheckHit(const RECT& rectSrc, float damage, Type hitType, POINT effectPoint)
{
	if (bossData.isCreated == false)
	{
		return false;
	}
	else if (bossData.isDeath == true)
	{
		return false;
	}

	const RECT rectBody = GameObject::GetRectBody();
	RECT rectInter = { 0, };
	if (rectSrc.top > rectBody.bottom)
	{
		return false;
	}
	else if (GameObject::IsCollide(rectSrc, &rectInter) == true)
	{
		if (effectPoint.x == -1)
		{
			const int interWidth = rectInter.right - rectInter.left;
			const int interHeight = rectInter.bottom - rectInter.top;
			const int randX = rectInter.left + (rand() % interWidth);
			const int randY = rectInter.top + (rand() % interHeight);

			effectPoint = { randX, randY };
		}
		else
		{
			const int maxYPos = rectBody.top + 30;
			if (maxYPos < rectSrc.top)
			{
				const int randHit = rand() % 12;
				if (randHit != 0)
				{
					return false;
				}
			}
		}

		EffectType effectType = GetEffectType_Hit(hitType);
		PushHitEffect(effectType, effectPoint);

		const float calDamage = CalculateDamage(damage, GetType(), hitType);
		if ((bossData.hp -= calDamage) <= 0)
		{
			Death();
		}
		return true;
	}

	return false;
}
//
//void Boss::Animate(const HWND& hWnd)
//{
//	constexpr int loadingFrame = -40;
//
//	if (IsCreated() == false)
//	{
//		return;
//	}
//	else if (bossData.isDeath == true)
//	{
//		if (--deathFrame > 0)
//		{
//			effects->CreateBossDeathEffect(*this);
//		}
//		else if (deathFrame == -1)
//		{
//			soundManager->PlayEffectSound(EffectSound::Win);
//			soundManager->StopBGMSound();
//
//			effects->CreateBossExplosionEffect(*this);
//		}
//		else if (deathFrame == loadingFrame)
//		{
//			phase.ClearPhase();
//			sceneManager->StartLoading(hWnd);
//		}
//		return;
//	}
//	else if (isRevFrame == true)
//	{
//		--frame;
//	}
//	else
//	{
//		++frame;
//	}
//
//	switch (GetAction())
//	{
//	case Action::Idle:
//		if (frame > bossData.frameNum_IdleMax)
//		{
//			if (bossData.type == Type::Dark)
//			{
//				frame = bossData.frameNum_Idle;
//			}
//			else
//			{
//				isRevFrame = true;
//				--frame;
//			}
//		}
//		else if (frame < bossData.frameNum_Idle)
//		{
//			isRevFrame = false;
//			++frame;
//		}
//		break;
//	case Action::Attack:
//		if (frame > bossData.frameNum_AtkMax)
//		{
//			isRevFrame = true;
//			--frame;
//		}
//		else if (isRevFrame == true && frame < bossData.frameNum_AtkRev)
//		{
//			isRevFrame = false;
//			if (bossData.type != Type::Dark)
//			{
//				SetAction(Action::Idle, bossData.frameNum_Idle);
//			}
//		}
//		break;
//	default:
//		assert(0);
//		break;
//	}
//}


//void Boss::AnimateSkill()
//{
//	if (IsCreated() == false)
//	{
//		return;
//	}
//	else if (IsDeath() == true)
//	{
//		return;
//	}
//
//	skill->Animate();
//}


void Boss::ShotByLine()
{
	constexpr int bulletCount = 7;

	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	const int bulletSizeX = bullets->GetBulletSize().x;
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2) + (((bulletCount / 2) * bulletSizeX) / 2);

	const int bulletMoveAmount = bulletSizeX / 20;
	const Vector2 unitVector = Vector2::Down();
	for (int i = 0; i < bulletCount; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		bulletPos.x -= bulletMoveAmount;
	}
}
void Boss::ShotByCircle()
{
	constexpr int bulletCount = 36;

	const BulletData bulletData = GetBulletData();

	const POINT bulletPos = GetPosCenter();

	Vector2 unitVector = Vector2::Up();
	unitVector = Rotate(unitVector, skillCount * 6); // Make different degree each fire
	for (int i = 0; i < bulletCount; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		unitVector = Rotate(unitVector, 360 / bulletCount);
	}
}
void Boss::ShotBySpiral()
{
	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	static int rotation = 8;
	static Vector2 unitVector = Vector2::Down();
	if (skillCount == 360)
	{
		unitVector = Vector2::Down();
	}
	if (abs(skillCount % (180 / rotation)) == 0)
	{
		rotation = ((rand() % 3) + 4) * 2;
	}
	unitVector = Rotate(unitVector, rotation);
	bullets->CreateBullet(bulletPos, bulletData, unitVector);
}
void Boss::ShotBySector()
{
	constexpr int bulletCount = 12;

	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	Vector2 unitVector = Vector2::Down();
	const float startDegree = 50 + (((skillCount % 4) * 16));
	const int rotationDegree = -(startDegree * 2) / bulletCount;
	unitVector = Rotate(unitVector, startDegree);
	for (int i = 0; i < bulletCount + 1; ++i)
	{
		bullets->CreateBullet(bulletPos, bulletData, unitVector);
		unitVector = Rotate(unitVector, rotationDegree);
	}
}
void Boss::ShotBySpread()
{
	const BulletData bulletData = GetBulletData();

	const RECT rectBody = GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.bottom;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	const int rotation = 30 + (rand() % 300);

	Vector2 unitVector = Rotate(Vector2::Up(), rotation);
	bullets->CreateBullet(bulletPos, bulletData, unitVector);
}

BossData Boss::GetBossData()
{
	Difficulty difficulty = Difficulty::Easy; //GetDifficulty();
	BossData bossData;

	bossData.speed = 1;

	bossData.bulletSpeed[static_cast<int>(BossAct::Line)] = 6;
	bossData.bulletSpeed[static_cast<int>(BossAct::Sector)] = 3;
	bossData.bulletSpeed[static_cast<int>(BossAct::Circle)] = 4;
	bossData.bulletSpeed[static_cast<int>(BossAct::Spiral)] = 5;
	bossData.bulletSpeed[static_cast<int>(BossAct::Spread)] = 6;

	bossData.attackDelay[static_cast<int>(BossAct::Line)] = 40;
	bossData.attackDelay[static_cast<int>(BossAct::Sector)] = 250;
	bossData.attackDelay[static_cast<int>(BossAct::Circle)] = 200;
	bossData.attackDelay[static_cast<int>(BossAct::Spiral)] = 10;
	bossData.attackDelay[static_cast<int>(BossAct::Spread)] = 10;

	bossData.frameNum_Idle = 0;
	switch (SCENE_MGR->GetCrntStageType())
	{
	case StageElement::Elec:
		bossData.type = Type::Elec;

		switch (difficulty)
		{
		case Difficulty::Easy:
			bossData.hp = 3000;
			bossData.damage = 2.0f;
			bossData.damage_skill1 = 12.5f;
			bossData.damage_skill2 = 1.75f;
			bossData.actDelay = 1550;
			break;
		case Difficulty::Normal:
			bossData.hp = 5000;
			bossData.damage = 2.8f;
			bossData.damage_skill1 = 15.5f;
			bossData.damage_skill2 = 2.5f;
			bossData.actDelay = 1250;
			break;
		case Difficulty::Hard:
			bossData.hp = 6000;
			bossData.damage = 3.25f;
			bossData.damage_skill1 = 17.5f;
			bossData.damage_skill2 = 3.0f;
			bossData.actDelay = 1100;
			break;
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 5;
		bossData.frameNum_AtkRev = 5;
		break;
	case StageElement::Water:
		bossData.type = Type::Water;

		switch (difficulty)
		{
		case Difficulty::Easy:
			bossData.hp = 2250;
			bossData.damage = 1.5f;
			bossData.damage_skill1 = 3.5f;
			bossData.damage_skill2 = 2.0f;
			bossData.actDelay = 2000;
			break;
		case Difficulty::Normal:
			bossData.hp = 3000;
			bossData.damage = 1.8f;
			bossData.damage_skill1 = 4.5f;
			bossData.damage_skill2 = 2.5f;
			bossData.actDelay = 1750;
			break;
		case Difficulty::Hard:
			bossData.hp = 4000;
			bossData.damage = 2.4f;
			bossData.damage_skill1 = 5.0f;
			bossData.damage_skill2 = 3.0f;
			bossData.actDelay = 1250;
			break;
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 2;
		bossData.frameNum_AtkMax = 3;
		bossData.frameNum_AtkRev = 3;
		break;
	case StageElement::Fire:
		bossData.type = Type::Fire;

		switch (difficulty)
		{
		case Difficulty::Easy:
			bossData.hp = 3000;
			bossData.damage = 2.0f;
			bossData.damage_skill1 = 5.5f;
			bossData.damage_skill2 = 9.5f;
			bossData.actDelay = 1750;
			break;
		case Difficulty::Normal:
			bossData.hp = 4000;
			bossData.damage = 2.2f;
			bossData.damage_skill1 = 7.5f;
			bossData.damage_skill2 = 15.0f;
			bossData.actDelay = 1500;
			break;
		case Difficulty::Hard:
			bossData.hp = 5000;
			bossData.damage = 2.75f;
			bossData.damage_skill1 = 9.0f;
			bossData.damage_skill2 = 16.0f;
			bossData.actDelay = 1250;
			break;
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 1;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 6;
		bossData.frameNum_AtkRev = 6;
		break;
	case StageElement::Dark:
		bossData.type = Type::Dark;

		switch (difficulty)
		{
		case Difficulty::Easy:
			bossData.hp = 5000;
			bossData.damage = 4.0f;
			bossData.damage_skill1 = 5.5f;
			bossData.damage_skill2 = 6.5f;
			bossData.actDelay = 1500;
			break;
		case Difficulty::Normal:
			bossData.hp = 6000;
			bossData.damage = 4.75f;
			bossData.damage_skill1 = 6.5f;
			bossData.damage_skill2 = 7.5f;
			bossData.actDelay = 1150;
			break;
		case Difficulty::Hard:
			bossData.hp = 7500;
			bossData.damage = 5.25f;
			bossData.damage_skill1 = 8.0f;
			bossData.damage_skill2 = 9.0f;
			bossData.actDelay = 950;
			break;
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 14;
		bossData.frameNum_Atk = 0;
		bossData.frameNum_AtkMax = 0;
		bossData.frameNum_AtkRev = 0;
		break;
	}
	bossData.crntActDelay = bossData.actDelay;

	//if (phase.GetPhase() == 3)
	//{
	//	bossData.hp += 1000;
	//}
	bossData.maxHP = bossData.hp;

	return bossData;
}