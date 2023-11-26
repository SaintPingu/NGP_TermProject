#pragma once
#include "Object.h"

typedef struct EnemyData {
	Type type = Type::Empty;

	int attackDelay = 0;
	int crntAttackDelay = 0;
	float bulletSpeed = 0;

	float hp = 0;
	float speed = 0;
	float damage = 0;

	int maxYPos = 0;
	int frameNum_Idle = 0;
	int frameNum_IdleMax = 0;
	int frameNum_Atk = 0;
	int frameNum_AtkMax = 0;
	int frameNum_AtkRev = 0;
}EnemyData;

class Enemy abstract : public GameObject, public IMovable {
protected:
	EnemyData data;
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	Dir GetDir() const;
	virtual void SetPosDest() abstract override;

	inline void ResetAttackDelay()
	{
		data.crntAttackDelay = data.attackDelay;
	}
	inline bool IsClearAttackDelay() const
	{
		return (data.crntAttackDelay <= 0);
	}
public:
	Enemy(ObjectImage& image, const Vector2& pos, const EnemyData& data);
	virtual void Move() override;
	virtual void CheckAttackDelay() abstract;

	bool Hit(float damage);

	inline Type GetType() const
	{
		return data.type;
	}
};



class EnemyBullet;
class BulletData;
class EnemyController {
private:
	std::vector<Enemy*> enemies;

	EnemyBullet* bullets = nullptr;

	EnemyData meleeData;
	EnemyData rangeData;

	float createDelay_Melee = 0;
	float createDelay_Range = 0;
	float delay_Melee = 0;
	float delay_Range = 0;
	int createAmount_Melee = 0;
	int createAmount_Range = 0;

	ObjectImage imgMelee;
	ObjectImage imgRange;

	void Pop(size_t& index);
public:
	EnemyController();
	~EnemyController();
	void CreateCheckMelee();
	void CreateCheckRange();
	void Move();
	bool CheckHit(const RECT& rectSrc, float damage, Type hitType, const POINT& effectPoint);
	void CheckHitAll(const RECT& rectSrc, float damage, Type hitType);

	void CreateBullet(const POINT& center, const BulletData& data, const Vector2& unitVector);
	void CreateBullet(const POINT& center, const BulletData& data, Dir dir);
	void MoveBullets();
	void DestroyCollideBullet(const RECT& rect);

	inline bool IsEmenyClear()
	{
		return enemies.empty();
	}
	inline void CheckAttackDelay()
	{
		for (Enemy* enemy : enemies)
		{
			enemy->CheckAttackDelay();
		}
	}
};