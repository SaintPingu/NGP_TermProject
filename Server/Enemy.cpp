#include "stdafx.h"
#include "Enemy.h"
#include "Bullet.h"
#include "DataBase.h"
#include "SceneMgr.h"
#include "ServerFramework.h"


Enemy::Enemy(ObjectImage& image, const Vector2& pos, const EnemyData& data) : GameObject(image, pos)
{
	StartMove();
	this->data = data;
}

Dir Enemy::GetDir() const
{
	const float theta = atan2(unitVector.y, unitVector.x);
	const float crntDegree = RADIAN_TO_DEGREE(theta);

	constexpr int unitDegree = 45;
	float degree = (float)unitDegree / 2;
	if (crntDegree > 0)
	{
		if (crntDegree < degree)
		{
			return Dir::Left;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::LU;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::Up;
		}
		else if (crntDegree < (degree += unitDegree))
		{
			return Dir::RU;
		}
		else
		{
			return Dir::Right;
		}
	}
	else
	{
		degree = -((float)unitDegree / 2);
		if (crntDegree > degree)
		{
			return Dir::Left;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::LD;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::Down;
		}
		else if (crntDegree > (degree -= unitDegree))
		{
			return Dir::RD;
		}
		else
		{
			return Dir::Right;
		}
	}
}


void Enemy::Move()
{
	if (IsMove() == false)
	{
		return;
	}

	SetPosDest();
	SetPos(posDest);
}

bool Enemy::Hit(float damage)
{
	if ((data.hp -= damage) <= 0)
	{
		return true;
	}

	return false;
}











void EnemyController::Pop(size_t& index)
{
	//effects->CreateExplosionEffect(enemies.at(index)->GetPosCenter(), enemies.at(index)->GetType());
	//soundManager->PlayEffectSound(EffectSound::Explosion);
	enemies[index--] = enemies.back();
	enemies.pop_back();
}
EnemyController::EnemyController()
{
	ObjectImage imgRangeBullet;
	switch (SCENE_MGR->GetCrntStageType()) {
	case StageElement::Elec:
		EnemyDataInit(meleeData, rangeData, Type::Elec);

		imgMelee.Load({ 33,33 }, { 21,22 });
		imgRange.Load({ 58,58 }, { 36,46 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRangeBullet.Load({ 14,14 });
		imgRangeBullet.ScaleImage(0.9f, 0.9f);

		createDelay_Melee = 1.5f;
		createDelay_Range = 3.f;
		createAmount_Melee = 6;
		createAmount_Range = 6;

		break;
	case StageElement::Water:
		EnemyDataInit(meleeData, rangeData, Type::Water);

		imgMelee.Load({ 34,33 }, { 28,22 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load({ 29,31 }, { 25,28 });
		imgRange.ScaleImage(1.2f, 1.2f);
		imgRangeBullet.Load({ 14,14 });
		imgRangeBullet.ScaleImage(1.2f, 1.2f);

		createDelay_Melee = 1.35f;
		createDelay_Range = 3.25f;
		createAmount_Melee = 10;
		createAmount_Range = 8;

		break;
	case StageElement::Fire:
		EnemyDataInit(meleeData, rangeData, Type::Fire);

		imgMelee.Load({ 37,37 }, {27,27 });
		imgMelee.ScaleImage(1.2f, 1.2f);
		imgRange.Load({ 44,34 }, { 42,29 });
		imgRange.ScaleImage(1.3f, 1.3f);
		imgRangeBullet.Load({ 14,14 });
		imgRangeBullet.ScaleImage(0.8f, 0.8f);

		createDelay_Melee = 2;
		createDelay_Range = 2.5;
		createAmount_Melee = 10;
		createAmount_Range = 6;

		break;
	case StageElement::Dark:
		EnemyDataInit(meleeData, rangeData, Type::Dark);

		imgMelee.Load({ 40,30 }, { 32,19 });
		imgMelee.ScaleImage(1.1f, 1.1f);
		imgRange.Load({ 40,40 }, { 30,27 });
		imgRange.ScaleImage(1.5f, 1.5f);
		imgRangeBullet.Load({ 10,10 });
		imgRangeBullet.ScaleImage(1.6f, 1.6f);


		createDelay_Melee = 2;
		createDelay_Range = 3;
		createAmount_Melee = 13;
		createAmount_Range = 6;

		break;
	default:
		assert(0);
		break;
	}

	const float randHP_Melee = (float)(rand() % 6) / 10;
	const float randHP_Range = (float)(rand() % 6) / 10;
	meleeData.hp += randHP_Melee;
	rangeData.hp += randHP_Range;

	bullets = new EnemyBullet(imgRangeBullet);
}
EnemyController::~EnemyController()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	delete bullets;
}


void EnemyController::CreateCheckMelee()
{
	//if (boss->IsCreated() == true)
	//{
	//	return;
	//}
	//else if (gui->IsFieldEnd() == true)
	//{
	//	return;
	//}

	delay_Melee += DeltaTime();
	if (delay_Melee < createDelay_Melee)
	{
		return;
	}
	delay_Melee = 0;

	for (int i = 0; i < createAmount_Melee; ++i)
	{
		float xPos = rand() % WINDOWSIZE_X;
		float yPos = -(rand() % 100);

		//Melee* enemy = new Melee(imgMelee, { xPos, yPos }, meleeData);
		//enemies.emplace_back(enemy);
	}
}
void EnemyController::CreateCheckRange()
{
	//if (boss->IsCreated() == true)
	//{
	//	return;
	//}
	//else if (gui->IsFieldEnd() == true)
	//{
	//	return;
	//}

	delay_Range += DeltaTime();
	if (delay_Range < createDelay_Range)
	{
		return;
	}
	delay_Range = 0;

	for (int i = 0; i < createAmount_Range; ++i)
	{
		rangeData.maxYPos = (rand() % 100) + 50;
		const float xPos = (rand() % (WINDOWSIZE_X - 100)) + 50;
		const float yPos = -(rand() % 100);

		/*Range* enemy = new Range(imgRange, { xPos, yPos }, rangeData);
		enemies.emplace_back(enemy);*/
	}
}


void EnemyController::Move()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Move();
	}
}

bool EnemyController::CheckHit(const RECT& rectSrc, float damage, Type hitType, const POINT& effectPoint)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies.at(i)->IsCollide(rectSrc) == true)
		{
			//effects->CreateHitEffect(effectPoint, hitType);
			const float calDamage = CalculateDamage(damage, enemies.at(i)->GetType(), hitType);
			if (enemies.at(i)->Hit(damage) == true)
			{
				EnemyController::Pop(i);
			}
			return true;
		}
	}

	return false;
}
void EnemyController::CheckHitAll(const RECT& rectSrc, float damage, Type hitType)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies.at(i)->IsCollide(rectSrc) == true)
		{
			POINT effectPoint = enemies.at(i)->GetPosCenter();
			/*GetRandEffectPoint(effectPoint);
			effects->CreateHitEffect(effectPoint, hitType);*/
			const float calDamage = CalculateDamage(damage, enemies.at(i)->GetType(), hitType);
			if (enemies.at(i)->Hit(calDamage) == true)
			{
				EnemyController::Pop(i);
			}
		}
	}
}

void EnemyController::CreateBullet(const POINT& center, const BulletData& data, const Vector2& unitVector)
{
	bullets->CreateBullet(center, data, unitVector);
}
void EnemyController::CreateBullet(const POINT& center, const BulletData& data, Dir dir)
{
	bullets->CreateBullet(center, data, dir);
}
void EnemyController::MoveBullets()
{
	bullets->Move();
}
void EnemyController::DestroyCollideBullet(const RECT& rect)
{
	bullets->DestroyCollideBullet(rect);
}