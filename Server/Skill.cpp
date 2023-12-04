#include "stdafx.h"
#include "Skill.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "bullet.h"
#include "Boss.h"
//#include "interface.h"
#include "scene.h"
//#include "sound.h"

#include "BattleScene.h"
#include "SceneMgr.h"
#include "ServerFramework.h"

//extern GameData gameData;
//extern Player* player;
//extern Boss* boss;
//extern EnemyController* enemies;
//extern EffectManager* effects;
//extern SCENE_MGR* SCENE_MGR;
//extern SoundManager* soundManager;

//extern Battle battle;

SkillManager::Effect::Effect(const EffectImage& imgSkill, Type type)
{
	this->imgSkill = &imgSkill;
	this->type = type;
}
//void SkillManager::Effect::Paint(const HDC& hdc, const RECT& rectBody) const
//{
//	RECT rectImage;
//	if (type == Type::Water)
//	{
//		rectImage = ISprite::GetRectImage(*imgSkill, 0);
//	}
//	else
//	{
//		rectImage = ISprite::GetRectImage(*imgSkill, frame);
//	}
//	imgSkill->Paint(hdc, rectBody, &rectImage);
////}
//bool SkillManager::Effect::Animate()
//{
//	static int frameLoopCount = 4;
//
//	if (++frame >= imgSkill->GetMaxFrame())
//	{
//		frame = 0;
//		frameLoopCount = 4;
//		return false;
//	}
//
//	switch (type)
//	{
//	case Type::Elec:
//	{
//		if (frame >= 8)
//		{
//			if (frameLoopCount-- > 0)
//			{
//				frame = 4;
//				return true;
//			}
//		}
//	}
//	break;
//	default:
//		break;
//	}
//
//	return true;
//}





SkillManager::SkillManager(Player* owner)
{
	player = owner;
	const Type type = owner->GetType();
	switch (type)
	{
	case Type::Elec:
		imgSkill_Elec_Q.Load({ 34,226 }, 10);

		//imgSkill_Elec_Q.Load(_T("images\\battle\\sprite_skill_elec.png"), { 34,226 }, 10, 0xdf);
		skillEffect = new Effect(imgSkill_Elec_Q, type);
		break;
	case Type::Fire:
		imgSkill_Fire_Q.Load({ 34,226 }, 10);

		//imgSkill_Fire_Q.Load(_T("images\\battle\\sprite_skill_fire.png"), { 80,96 }, 50);
		skillEffect = new Effect(imgSkill_Fire_Q, type);
		break;
	case Type::Water:
		imgSkill_Water_Q.Load({ 34,226 }, 10);

		//imgSkill_Water_Q.Load(_T("images\\battle\\skill_water.png"), { 273,843 }, 50, 0xaf);
		skillEffect = new Effect(imgSkill_Water_Q, type);
		break;
	default:
		assert(0);
		break;
	}
}

RECT SkillManager::GetRectBody() const
{
	RECT rectBody = { 0, };
	switch (player->GetType())
	{
	case Type::Elec:
		rectBody = player->GetRectBody();
		rectBody.left -= 20;
		rectBody.right += 20;
		rectBody.bottom = rectBody.top;
		rectBody.top = rectBody.bottom - WINDOWSIZE_Y;
		break;
	case Type::Fire:
		rectBody = player->GetRectBody();
		rectBody.left -= 40;
		rectBody.right += 30;
		rectBody.bottom = rectBody.top + 10;
		rectBody.top = rectBody.bottom - 400;
		break;
	case Type::Water:
	{
		const int maxFrame = imgSkill_Water_Q.GetMaxFrame();
		const int frame = skillEffect->GetFrame();
		rectBody.left = 0;
		rectBody.right = WINDOWSIZE_X;
		rectBody.top = WINDOWSIZE_Y - (((float)frame / maxFrame) * WINDOWSIZE_Y * 2);
		rectBody.bottom = rectBody.top + WINDOWSIZE_Y;
	}
	break;
	default:
		assert(0);
		break;
	}

	return rectBody;
}

void SkillManager::UseSkill()
{
	if (IsUsingSkill() == false)
	{
		return;
	}

	switch (crntSkill)
	{
	case Skill::Sector:
		ShotBySector();
		break;
	case Skill::Circle:
		ShotByCircle();
		break;
	default:
		assert(0);
		break;
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}

void SkillManager::Update()
{
	if (isIdentity == false)
	{
		return;
	}
	//else if (skillEffect->Animate() == false)
	//{
	//	isIdentity = false;
	//	return;
	//}

	RECT rectBody = GetRectBody();
	rectBody.top += 20;
	const Type playerType = player->GetType();
	if (playerType == Type::Fire)
	{
		if (skillEffect->GetFrame() < 17)
		{
			rectBody.left += (rectBody.right - rectBody.left) / 2;
			rectBody.top += 100;
		}
		else if (skillEffect->GetFrame() > 27)
		{
			rectBody.right -= (rectBody.right - rectBody.left) / 2;
		}
	}

	const float damage = player->GetDamage_Q();
	//for (int i = 0; i < 5; ++i)
	//{
	//	if (boss->CheckHit(rectBody, damage, playerType) == false)
	//	{
	//		break;
	//	}
	//}

	auto& enemies = SCENE_MGR->Battle()->GetEnemyController();
	enemies->CheckHitAll(rectBody, damage, playerType);
	if (playerType == Type::Fire ||
		playerType == Type::Elec)
	{
		enemies->DestroyCollideBullet(rectBody);
	}
}

void SkillManager::ActiveSkill(Skill skill)
{
	if (skill == Skill::Identity && isIdentity == true)
	{
		return;
	}
	else if (skill != Skill::Identity && IsUsingSkill() == true)
	{
		return;
	}

	switch (skill)
	{
	case Skill::Sector:
		if (player->ReduceMP(15) == false)
		{
			return;
		}
		skillCount = 7;
		crntSkill = skill;
		break;
	case Skill::Circle:
		if (player->ReduceMP(10) == false)
		{
			return;
		}
		skillCount = 10;
		crntSkill = skill;
		break;
	case Skill::Identity:
		if (player->ReduceMP(30) == false)
		{
			return;
		}

		isIdentity = true;
		break;
	default:
		assert(0);
		break;
	}
}

void SkillManager::ShotBySector()
{
	constexpr int bulletCount = 12;

	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	const RECT rectBody = player->GetRectBody();
	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);

	Vector2 unitVector = Vector2::Up();
	const float startDegree = 10 + (skillCount * 10);
	const float rotationDegree = -(startDegree * 2) / bulletCount;
	unitVector = Rotate(unitVector, startDegree);

	for (int i = 0; i < bulletCount + 1; ++i)
	{
		player->GetSubBullets()->CreateBullet(bulletPos, bulletData, unitVector, true, true);
		unitVector = Rotate(unitVector, rotationDegree);
	}
}
void SkillManager::ShotByCircle()
{
	constexpr int bulletCount = 18;

	BulletData bulletData;
	bulletData.bulletType = player->GetSubType();
	bulletData.damage = player->GetDamage_WE();
	bulletData.speed = 10;

	const POINT bulletPos = player->GetPosCenter();

	Vector2 unitVector = Vector2::Up();
	constexpr int degree = 6;
	unitVector = Rotate(unitVector, degree * skillCount); // Make different degree each fire

	for (int i = 0; i < bulletCount; ++i)
	{
		player->GetSubBullets()->CreateBullet(bulletPos, bulletData, unitVector, true, true);
		unitVector = Rotate(unitVector, 360 / bulletCount);
	}
}



//RECT BossSkillManager::Effect::GetRectBody() const
//{
//	const POINT size = imgSkill.GetDrawSize();
//	RECT rectBody = { 0, };
//	rectBody.left = posCenter.x - ((float)size.x / 2);
//	rectBody.top = posCenter.y - ((float)size.y / 2);
//	rectBody.right = rectBody.left + size.x;
//	rectBody.bottom = rectBody.top + size.y;
//
//	return rectBody;
//}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const FRECT rectDraw, const SkillData& skillData)
{
	this->imgSkill = imgSkill;
	this->rectDraw = rectDraw;
	this->skillData = skillData;
}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const Vector2& pos, const SkillData& skillData)
{
	this->imgSkill = imgSkill;
	this->posCenter = pos;
	this->skillData = skillData;
}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const Vector2& pos, float rotationDegree, const SkillData& skillData) : Effect(imgSkill, pos, skillData)
{
	if (rotationDegree != 0)
	{
		this->skillData.isRotated = true;
		unitVector_imgRotation = ::Rotate(Vector2::Down(), rotationDegree);
	}
}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const Vector2& pos, const Vector2& unitVector_imgRotation, const SkillData& skillData) : Effect(imgSkill, pos, skillData)
{
	if (unitVector_imgRotation != Vector2::Down())
	{
		this->skillData.isRotated = true;
	}
	else
	{
		this->skillData.isRotated = false;
	}

	this->unitVector_imgRotation = unitVector_imgRotation;
}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const Vector2& pos, const Vector2& unitVector_imgRotation, const Vector2& unitVector_direction, const SkillData& skillData) : Effect(imgSkill, pos, unitVector_imgRotation, skillData)
{
	this->unitVector_direction = unitVector_direction;
}
BossSkillManager::Effect::Effect(const EffectImage& imgSkill, const SkillData& skillData, const DarkSkillData& darkSkillData)
{
	/*this->imgSkill = imgSkill;*/
	this->skillData = skillData;
	this->darkSkillData = darkSkillData;

	posCenter = darkSkillData.GetPosCenter();
}


//void BossSkillManager::Effect::Paint(HDC hdc) const
//{
//	const RECT rectBody = GetRectBody();
//	RECT rectImage = { 0, };
//	if (skillData.damage == 0)
//	{
//		rectImage = imgSkill.GetRectImage();
//	}
//	else
//	{
//		rectImage = ISprite::GetRectImage(imgSkill, frame);
//
//	}
//
//	if (rectDraw.left != -1)
//	{
//		imgSkill.Paint(hdc, rectDraw, &rectImage);
//	}
//	else if (skillData.isRotated == false)
//	{
//		imgSkill.Paint(hdc, rectBody, &rectImage);
//	}
//	else
//	{
//		Vector2 vPoints[4];
//		if (unitVector_direction == Vector2::Zero())
//		{
//			GetRotationPos(rectBody, unitVector_imgRotation, Vector2::Up(), vPoints);
//		}
//		else
//		{
//			GetRotationPos(rectBody, unitVector_direction, Vector2::Up(), vPoints);
//		}
//		imgSkill.PaintRotation(hdc, vPoints, &rectImage);
//
//		if (gameData.isShowHitbox == true)
//		{
//			MoveToEx(hdc, posCenter.x, posCenter.y, NULL);
//			LineTo(hdc, posCenter.x + (unitVector_direction.x * 100), posCenter.y + (unitVector_direction.y * 100));
//		}
//	}
//}


bool BossSkillManager::Effect::Animate(Player* player)
{
	const int maxFrame = imgSkill.GetMaxFrame();
	if (maxFrame > 0 && ++frame >= maxFrame)
	{
		return false;
	}
	else if (skillData.damage > 0 && skillData.isActiveDamage == true)
	{
		RECT rectBody = GetRectBody();

		if (skillData.isRotated == true)
		{
			Vector2 vPoints[4];
			GetRotationPos(rectBody, unitVector_imgRotation, Vector2::Up(), vPoints);
			if (SATIntersect(player->GetRectBody(), vPoints) == true)
			{
				player->Hit(skillData.damage, boss->GetType());
				if (skillData.isHitOnce == true)
				{
					return false;
				}
			}
		}
		else
		{
			if (rectDraw.left != -1)
			{
				rectBody = rectDraw;
			}

			const RECT rectPlayer = player->GetRectBody();
			if (IntersectRect2(rectBody, rectPlayer) == true)
			{
				player->Hit(skillData.damage, boss->GetType());
				if (skillData.isHitOnce == true)
				{
					return false;
				}
			}
		}

	}

	return Move(boss);
}


bool BossSkillManager::Effect::Move(Boss* boss)
{
	if (unitVector_direction == Vector2::Zero())
	{
		if (darkSkillData.posOrigin != nullptr)
		{
			constexpr float rotationDegree = 3.0f;
			if (*darkSkillData.posOrigin == Vector2::Zero())
			{
				return false;
			}
			posCenter = darkSkillData.GetRotatePos(rotationDegree);
		}

		return true;
	}

	posCenter = posCenter + (unitVector_direction * skillData.speed);
	const RECT rectDisplay = SCENE_MGR->Battle()->GetRectDisplay();
	const RECT rectBody = GetRectBody();

	if (boss->GetType() == Type::Dark)
	{
		if (skillData.damage > 0)
		{
			if (OutOfRange(rectBody, rectDisplay) == true)
			{
				return false;
			}
		}
	}
	else if (rectBody.top > rectDisplay.bottom)
	{
		return false;
	}

	return true;
}
RECT BossSkillManager::Effect::GetRectBody() const
{
	return RECT();
}
bool BossSkillManager::Effect::RotateToPlayer(float t, Player* player)
{
	constexpr int maxRotation = 35;
	constexpr int maxMove = 80;
	skillData.isRotated = true;

	if (skillData.rotationCount++ < maxRotation)
	{
		::Rotate(posCenter, player->GetPosCenter(), unitVector_direction, t);
	}
	else if (skillData.rotationCount > maxMove)
	{
		return false;
	}

	return true;
}








BossSkillManager::BossSkillManager()
{
	boss = SCENE_MGR->Battle()->GetBoss();
	switch (boss->GetType())
	{
	case Type::Elec:
		imgSkill1.Load({ 32,224 }, 9);
		imgSkill1.ScaleImage(1.0f, 6.0f);

		imgSkill1_Warning.Load({ 31,223 }, 16);
		imgSkill1_Warning.ScaleImage(1.0f, 6.0f);

		imgSkill2.Load({ 80,80 }, 15);
		imgSkill2.ScaleImage(0.5f, 0.5f);

		imgSkill2_Warning.Load({ 79,79 }, 15);
		imgSkill2_Warning.ScaleImage(0.5f, 0.5f);

		//imgSkill1.Load(_T("images\\battle\\sprite_boss_skill1_elec.png"), { 32,224 }, 9);
		//imgSkill1.ScaleImage(1.0f, 6.0f);
		//imgSkill1_Warning.Load(_T("images\\battle\\boss_skill1_elec_warning.png"), { 31,223 }, 16, 0x10);
		//imgSkill1_Warning.ScaleImage(1.0f, 6.0f);
		//imgSkill2.Load(_T("images\\battle\\sprite_boss_skill2_elec.png"), { 80,80 }, 15);
		//imgSkill2.ScaleImage(0.5f, 0.5f);
		//imgSkill2_Warning.Load(_T("images\\battle\\boss_warning_circle.png"), { 79,79 }, 15, 0x00);
		//imgSkill2_Warning.ScaleImage(0.5f, 0.5f);
		break;
	case Type::Water:

		imgSkill1.Load({ 273,712 });
		imgSkill1.ScaleImage(0.25f, 0.25f);

		imgSkill2.Load({ 56,56 }, 14);
		imgSkill2.ScaleImage(0.5f, 0.5f);

		imgSkill2_Warning.Load({ 79,79 }, 15);
		imgSkill2_Warning.ScaleImage(0.5f, 0.5f);

	/*	imgSkill1.Load(_T("images\\battle\\boss_skill1_water.png"), { 273,712 });
		imgSkill1.ScaleImage(0.25f, 0.25f);
		imgSkill2.Load(_T("images\\battle\\sprite_boss_skill2_water.png"), { 56,56 }, 14);
		imgSkill2.ScaleImage(0.5f, 0.5f);
		imgSkill2_Warning.Load(_T("images\\battle\\boss_warning_circle.png"), { 79,79 }, 15, 0x00);
		imgSkill2_Warning.ScaleImage(0.5f, 0.5f);*/
		break;
	case Type::Fire:
		imgSkill1.Load({ 17,30 });
		imgSkill1.ScaleImage(2.0f, 2.0f);

		imgSkill2.Load({ 173,230 });
		imgSkill2.ScaleImage(1.5f, 1.5f);

		imgSkill2_Warning.Load({ 11,223 }, 8);
		imgSkill2_Warning.ScaleImage(1.0f, 6.0f);

		//imgSkill1.Load(_T("images\\battle\\boss_skill1_fire.png"), { 17,30 });
		//imgSkill1.ScaleImage(2.0f, 2.0f);
		//imgSkill2.Load(_T("images\\battle\\boss_skill2_fire.png"), { 173,230 });
		//imgSkill2.ScaleImage(1.5f, 1.5f);
		//imgSkill2_Warning.Load(_T("images\\battle\\boss_skill2_fire_warning.png"), { 11,223 }, 8, 0x00);
		//imgSkill2_Warning.ScaleImage(1.0f, 6.0f);
		break;
	case Type::Dark:
		imgSkill1.Load({ 699,699 });
		imgSkill1.ScaleImage(0.05f, 0.05f);

		imgSkill2.Load({ 11,33 });
		imgSkill2.ScaleImage(1.5f, 1.5f);

		imgSkill2_Warning.Load({ 100,100 }, 200);
		imgSkill2_Warning.ScaleImage(3.0f, 3.0f);

		posOrigins = new Vector2[dark_Skill1Count];
	/*	imgSkill1.Load(_T("images\\battle\\bullet_boss_dark.png"), { 699,699 });
		imgSkill1.ScaleImage(0.05f, 0.05f);
		imgSkill2.Load(_T("images\\battle\\boss_skill2_dark.png"), { 11,33 });
		imgSkill2.ScaleImage(1.5f, 1.5f);
		imgSkill2_Warning.Load(_T("images\\battle\\boss_skill2_dark_warning.png"), { 100,100 }, 200, 0x50);
		imgSkill2_Warning.ScaleImage(3.0f, 3.0f);*/
		break;
	default:
		assert(0);
		break;
	}
}
BossSkillManager::~BossSkillManager()
{
	if (boss->GetType() == Type::Dark)
	{
		delete[] posOrigins;
	}
}