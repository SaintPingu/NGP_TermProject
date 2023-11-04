#pragma once
#include "stdafx.h"


struct PlayerData {
	Type type;
	Type subType;
	Vector2 pos;
};

struct EnemyData {
	EnemyType enemyType;
	Vector2 pos;
};

struct BulletData {
	BulletType enemyType;
	Vector2 pos;
	Vector2 dir;
};

