
#include "stdafx.h"
#include "DataBase.h"

SINGLETON_PATTERN_DEFINITION(DataBase);

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

void DataBase::Init()
{
	PlayerDataInit(playerWaterData, Type::Water, Type::Water);
	PlayerDataInit(playerFireData, Type::Fire, Type::Fire);
	PlayerDataInit(playerElecData, Type::Elec, Type::Elec);

	EnemyDataInit(enemyWaterMeleeData, enemyWaterRangeData,Type::Water);
	EnemyDataInit(enemyFireMeleeData, enemyFireRangeData, Type::Fire);
	EnemyDataInit(enemyElecMeleeData, enemyElecRangeData, Type::Elec);
	EnemyDataInit(enemyDarkMeleeData, enemyDarkRangeData, Type::Dark);

	BossDataInit(bossWaterData, Type::Water, Difficulty::Easy);
	BossDataInit(bossFireData, Type::Fire, Difficulty::Easy);
	BossDataInit(bossElecData, Type::Elec, Difficulty::Easy);
	BossDataInit(bossDarkData, Type::Dark, Difficulty::Easy);

	RectInit(buildingLocations);
}

void PlayerDataInit(PlayerData& playerData, Type type, Type subType)
{
	{
		constexpr int damagePerSec = (1000 / ELAPSE_BATTLE_ANIMATION);

		playerData.type = type;
		playerData.subType = subType;
		switch (playerData.type)
		{
		case Type::Elec:
			playerData.maxhp = 40;
			playerData.maxmp = 80;
			playerData.mp = 0;
			playerData.speed = 4;
			playerData.damage = 1.0f;
			playerData.damage_Q = 10.5f / damagePerSec;
			playerData.bulletSpeed = 8;
			playerData.shotDelay = 90;
			break;
		case Type::Water:
			playerData.maxhp = 65;
			playerData.maxmp = 120;
			playerData.mp = 40;
			playerData.speed = 2.5f;
			playerData.damage = 1.25f;
			playerData.damage_Q = 4.25f / damagePerSec;
			playerData.bulletSpeed = 6;
			playerData.shotDelay = 110;
			break;
		case Type::Fire:
			playerData.maxhp = 50;
			playerData.maxmp = 100;
			playerData.mp = 20;
			playerData.speed = 3;
			playerData.damage = 1.35f;
			playerData.damage_Q = 12.5f / damagePerSec;
			playerData.bulletSpeed = 7;
			playerData.shotDelay = 100;
			break;
		default:
			assert(0);
			break;
		}
		playerData.hp = playerData.maxhp;

		switch (playerData.subType)
		{
		case Type::Elec:
			playerData.subDamage = 1.0f;
			break;
		case Type::Water:
			playerData.subDamage = 1.1f;
			break;
		case Type::Fire:
			playerData.subDamage = 1.2f;
			break;
		default:
			assert(0);
			break;
		}
	}
}

void EnemyDataInit(EnemyData& meleeData, EnemyData& rangeData, Type type)


{
	switch (type)
	{
	case Type::Elec: // StageElement -> Type
		meleeData.type = Type::Elec;
		rangeData.type = Type::Elec;

		meleeData.hp = 6;
		meleeData.speed = 1.5f;
		meleeData.attackDelay = 1000;
		meleeData.damage = 3;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 2;
		meleeData.frameNum_Atk = 3;
		meleeData.frameNum_AtkMax = 4;

		rangeData.hp = 4.75f;
		rangeData.speed = 2;
		rangeData.attackDelay = 2000;
		rangeData.damage = 2.25f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 2;
		rangeData.frameNum_Atk = 3;
		rangeData.frameNum_AtkMax = 4;
		rangeData.frameNum_AtkRev = 4;
		rangeData.bulletSpeed = 4;
		meleeData.frameNum_AtkRev = 3;
		break;
	case Type::Water:
		meleeData.type = Type::Water;
		rangeData.type = Type::Water;

		meleeData.hp = 2.85f;
		meleeData.speed = 2;
		meleeData.attackDelay = 700;
		meleeData.damage = 2.5f;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 2;
		meleeData.frameNum_Atk = 3;
		meleeData.frameNum_AtkMax = 4;
		meleeData.frameNum_AtkRev = 3;

		rangeData.hp = 5;
		rangeData.speed = 0.7f;
		rangeData.attackDelay = 2000;
		rangeData.damage = 2.8f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 2;
		rangeData.frameNum_Atk = 3;
		rangeData.frameNum_AtkMax = 6;
		rangeData.frameNum_AtkRev = 3;
		rangeData.bulletSpeed = 3;
		break;
	case Type::Fire:
		meleeData.type = Type::Fire;
		rangeData.type = Type::Fire;

		meleeData.hp = 6;
		meleeData.speed = 1.65f;
		meleeData.attackDelay = 1250;
		meleeData.damage = 2.0f;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 1;
		meleeData.frameNum_Atk = 2;
		meleeData.frameNum_AtkMax = 5;
		meleeData.frameNum_AtkRev = 5;

		rangeData.hp = 6.75f;
		rangeData.speed = 0.5f;
		rangeData.attackDelay = 1250;
		rangeData.damage = 3.0f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 0;
		rangeData.frameNum_Atk = 1;
		rangeData.frameNum_AtkMax = 2;
		rangeData.frameNum_AtkRev = 2;
		rangeData.bulletSpeed = 3.5f;
		break;
	case Type::Dark:
		meleeData.type = Type::Dark;
		rangeData.type = Type::Dark;

		meleeData.hp = 4.0f;
		meleeData.speed = 2.25f;
		meleeData.attackDelay = 850;
		meleeData.damage = 2.0f;

		meleeData.frameNum_Idle = 0;
		meleeData.frameNum_IdleMax = 2;
		meleeData.frameNum_Atk = 2;
		meleeData.frameNum_AtkMax = 4;
		meleeData.frameNum_AtkRev = 4;

		rangeData.hp = 8.5f;
		rangeData.speed = 0.75f;
		rangeData.attackDelay = 2000;
		rangeData.damage = 3.0f;

		rangeData.frameNum_Idle = 0;
		rangeData.frameNum_IdleMax = 2;
		rangeData.frameNum_Atk = 2;
		rangeData.frameNum_AtkMax = 4;
		rangeData.frameNum_AtkRev = 4;
		rangeData.bulletSpeed = 3.0f;
		break;
	default:
		assert(0);
		break;
	}

	const float randHP_Melee = (float)(rand() % 6) / 10;
	const float randHP_Range = (float)(rand() % 6) / 10;
	meleeData.hp += randHP_Melee;
	rangeData.hp += randHP_Range;

}

void BossDataInit(BossData& bossData, Type type, Difficulty difficulty)

{
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
	switch (type)
	{
	case Type::Elec: // StageElement -> Type
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
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 5;
		bossData.frameNum_AtkRev = 5;
		break;
	case Type::Water:
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
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 2;
		bossData.frameNum_Atk = 2;
		bossData.frameNum_AtkMax = 3;
		bossData.frameNum_AtkRev = 3;
		break;
	case Type::Fire:
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
		default:
			assert(0);
			break;
		}

		bossData.frameNum_IdleMax = 1;
		bossData.frameNum_Atk = 1;
		bossData.frameNum_AtkMax = 6;
		bossData.frameNum_AtkRev = 6;
		break;
	case Type::Dark:
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

	/*if (phase.GetPhase() == 3) // Phase Class 작성시 해제
	{
		bossData.hp += 1000;
	}*/

	bossData.maxHP = bossData.hp;

}

void RectInit(RECT* _object)
{
	// 왼쪽 상단부터
	_object[0] = { 0, 0, 125, 190 };
	_object[1] = { 156, 0, 300, 100 };
	_object[2] = { 337, 0, 750, 175 };
	_object[3] = { 644, 175, 750, 300 };
	_object[4] = { 0, 253, 144, 750 };
	_object[5] = { 130, 556, 267, 750 };
	_object[6] = { 267, 594, 325, 750 };
	_object[7] = { 325, 619, 362, 750 };
	_object[8] = { 687, 487, 750, 750 };
	_object[9] = { 725, 300, 750, 372 };
	_object[10] = { 218, 290, 247, 312 };
	_object[11] = { 306, 290, 334, 312 };
	_object[12] = { 184, 394, 312, 481 };
	_object[13] = { 500, 312, 600, 406 };
	_object[14] = { 184, 325, 187, 481 };
	_object[15] = { 506, 500, 594, 594 };
	_object[16] = { 0, 710, 750, 750 };
	_object[17] = { 189, 80, 191, 100 };
}