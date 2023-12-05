#pragma once
#include "object.h"

enum class BossAct { Line = 0, Circle, Spiral, Sector, Spread, Idle };

struct BulletData;
class ObjectImage;
class EnemyBullet;
class BossSkillManager;

#define BOSS_BULLET_LIST 5
#define BOSS_SKILL_LIST 8

typedef struct BossData {
	int frameNum_Idle = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk = 0;
	int frameNum_AtkMax = 0;
	int frameNum_AtkRev = 0;
}BossData;

class Boss : public GameObject, public IAnimatable {
private:
	BossData bossData;
	ObjectImage* image = nullptr;
	BossAct act = BossAct::Idle;

	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	BossSkillManager* skill = nullptr;

public:
	Boss();
	~Boss();
	void Render(HDC hdc);

	void Animate(const HWND& hWnd);
};
