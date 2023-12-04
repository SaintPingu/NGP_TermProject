#pragma once
#include "object.h"
#include "DataBase.h"

//enum class BossAct { Line = 0, Circle, Spiral, Sector, Spread, Skill1, Skill2, Idle };

struct BulletData;
class ObjectImage;
class EnemyBullet;
class BossSkillManager;
class Player;

//#define BOSS_BULLET_LIST 5
#define BOSS_SKILL_LIST 8

//typedef struct BossData {
//	Type type = Type::Empty;
//
//	int actDelay = 0;
//	int crntActDelay = 0;
//	int attackDelay[BOSS_BULLET_LIST] = { 0, };
//	int crntAttackDelay = 0;
//
//	float maxHP = 0;
//	float hp = 0;
//	float damage = 0;
//	float damage_skill1 = 0;
//	float damage_skill2 = 0;
//	float speed = 0;
//	float bulletSpeed[BOSS_BULLET_LIST] = { 0, };
//
//	int frameNum_Idle = 0;
//	int frameNum_IdleMax = 0;
//	int frameNum_Atk = 0;
//	int frameNum_AtkMax = 0;
//	int frameNum_AtkRev = 0;
//
//	bool isCreated = false;
//	bool isDeath = false;
//}BossData;

class Boss : public GameObject/*, public IAnimatable*/, public IMovable {
private:
	BossData bossData;
	ObjectImage* image   = nullptr;
	std::shared_ptr<EnemyBullet> bullets{};
	BossAct act;

	Vector2 posDest    = { 0, };
	Vector2 unitVector = { 0, };

	std::shared_ptr<BossSkillManager> skill{};
	int maxSkillCount[BOSS_SKILL_LIST] = { 0, };
	int skillCount                     = 0;
	int deathFrame                     = 30;
	int normalSkillCount               = 0;
	bool isShowHP                      = false;

	void SetMove(const Vector2& unitVector);
	void SetPosDest() override;
	void Death();
	void Shot();
	BulletData GetBulletData();

	void ResetAttackDelay();
	inline bool IsClearAttackDelay()
	{
		return (bossData.crntAttackDelay <= 0);
	}

	void ShotByLine();
	void ShotByCircle();
	void ShotBySpiral();
	void ShotBySector();
	void ShotBySpread();

	BossData GetBossData();
public:
	Boss();
	~Boss();
	void Create();
	void Move() override;
	void CheckAttackDelay();


	bool CheckHit(const RECT& rectSrc, float damage, Type hitType, POINT effectPoint = { -1, });

	const std::shared_ptr<EnemyBullet>& GetBullets() const { return bullets; }

	inline constexpr void ShowHP()
	{
		isShowHP = !isShowHP;
	}
	inline constexpr Type GetType() const
	{
		return bossData.type;
	}
	inline constexpr bool IsCreated() const
	{
		return bossData.isCreated;
	}
	inline constexpr bool IsDeath() const
	{
		return bossData.isDeath;
	}

	inline constexpr void KillBoss()
	{
		bossData.hp = 1.0f;
	}
};
