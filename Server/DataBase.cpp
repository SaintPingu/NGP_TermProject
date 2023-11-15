
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

