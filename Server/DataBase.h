#pragma once


// 2023-11-08-WED (장재문) : 최정일님 코드 일부 수정 - 사유 : 코드 재정의 오류 ( Common.h 에 이미 Type 정의 되어있음 ) - 주석 해결 + Init 함수들 cpp 에서 h 로 옮김 + stdafx.h 를 cpp 로 옮김  
//enum class Type { Empty = 0, Fire, Elec, Water, Dark }; 
enum class BossAct { Line = 0, Circle, Spiral, Sector, Spread, Skill1, Skill2, Idle };
enum class Difficulty { Easy = 0, Normal, Hard };

#define BOSS_BULLET_LIST 5
#define ELAPSE_BATTLE_ANIMATION 50
#define TOWN_OBJECT_NUM 18

typedef struct PlayerData {
	Type type = Type::Empty;
	Type subType = Type::Empty;

	float maxhp = 0;
	float maxmp = 0;
	float hp    = 0;
	float mp    = 0;
	float speed = 0;

	float bulletSpeed   = 0;
	float shotDelay     = 0;
	float crntShotDelay = 0;
	float damage        = 0;
	float subDamage     = 0;
	float damage_Q      = 0; // per sec

	bool isDeath      = false;
	bool isInvincible = false;
}PlayerData;

typedef struct BossData {
	Type type = Type::Empty;

	int actDelay                      = 0;
	int crntActDelay                  = 0;
	int attackDelay[BOSS_BULLET_LIST] = { 0, };
	int crntAttackDelay               = 0;

	float maxHP                         = 0;
	float hp                            = 0;
	float damage                        = 0;
	float damage_skill1                 = 0;
	float damage_skill2                 = 0;
	float speed                         = 0;
	float bulletSpeed[BOSS_BULLET_LIST] = { 0, };

	int frameNum_Idle    = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk     = 0;
	int frameNum_AtkMax  = 0;
	int frameNum_AtkRev  = 0;

	bool isCreated = false;
	bool isDeath   = false;
}BossData;

typedef struct EnemyData {
	Type type = Type::Empty;

	int attackDelay     = 0;
	int crntAttackDelay = 0;
	float bulletSpeed   = 0;

	float hp     = 0;
	float speed  = 0;
	float damage = 0;

	int maxYPos          = 0;
	int frameNum_Idle    = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk     = 0;
	int frameNum_AtkMax  = 0;
	int frameNum_AtkRev  = 0;
}EnemyData;

#include "windef.h"
class DataBase
{
	SINGLETON_PATTERN(DataBase);

public:
	DataBase();
	~DataBase();

	void Init();

	//player
	PlayerData playerWaterData;
	PlayerData playerFireData;
	PlayerData playerElecData;

	//enemy
	EnemyData enemyWaterMeleeData;
	EnemyData enemyWaterRangeData;
	EnemyData enemyFireMeleeData;
	EnemyData enemyFireRangeData;
	EnemyData enemyElecMeleeData;
	EnemyData enemyElecRangeData;
	EnemyData enemyDarkMeleeData;
	EnemyData enemyDarkRangeData;

	//boss
	BossData bossWaterData;
	BossData bossFireData;
	BossData bossElecData;
	BossData bossDarkData;

	//RECT
	RECT buildingLocations[TOWN_OBJECT_NUM];

};


void PlayerDataInit(PlayerData& playerData, Type type, Type subType);
void EnemyDataInit(EnemyData& meleeData, EnemyData& rangeData, Type type);
void BossDataInit(BossData& bossData, Type type, Difficulty difficulty);
void RectInit(RECT* _object);

