#pragma once
#include "object.h"

struct BulletData;
class Player;
//class EnemyBullet;

typedef struct EnemyDataStatus {
	int frameNum_Idle = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk = 0;
	int frameNum_AtkMax = 0;
	int frameNum_AtkRev = 0;
}EnemyData;

class Enemy abstract : public GameObject, public IAnimatable {
protected:
	EnemyData data{};
	Dir dir{};
	bool isAction{};

	void Render(const HDC& hdc, int spriteRow);
public:
	Enemy(ObjectImage& image, const Vector2& pos);
	virtual void Render(const HDC& hdc) abstract;

	int GetSpriteRow();
	void Animate();

	void SetDir(Dir val) {
		dir = val;
	}
	void SetIsAction(bool val) {
		isAction = val;
	}

};

class Melee : public Enemy {
public:
	Melee(ObjectImage& image, const Vector2& pos) : Enemy(image, pos) {}
	void Render(const HDC& hdc) override;
};

class Range : public Enemy {
private:
	const ObjectImage* bulletImage = nullptr;

public:
	Range(ObjectImage& image, const Vector2& pos) : Enemy(image, pos) {}
	void Render(const HDC& hdc) override;
};