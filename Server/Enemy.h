#pragma once
#include "Object.h"
#include "DataBase.h"
#include "Player.h"

class Enemy abstract : public GameObject, public IMovable {
protected:
	int ID{};
	EnemyData data;
	Vector2 posDest = { 0, };
	Vector2 unitVector = { 0, };

	virtual void SetPosDest() abstract override;

	inline void ResetAttackDelay()
	{
		data.crntAttackDelay = data.attackDelay;
	}
	inline bool IsClearAttackDelay() const
	{
		return (data.crntAttackDelay <= 0);
	}

	bool isAction{};
public:
	Enemy(ObjectImage& image, const Vector2& pos, const EnemyData& data);
	virtual void Move() override;
	virtual void CheckAttackDelay() abstract;

	bool Hit(float damage);

	inline Type GetType() const
	{
		return data.type;
	}

	virtual EnemyType GetEnemyType() const abstract;

	Dir GetDir() const;
	bool IsAction() const
	{
		return isAction;
	}

	void SetID(int _ID) { ID = _ID; }
	int GetID() const { return ID; }

};

class Melee : public Enemy {
private:
	void SetPosDest();
	bool CheckCollidePlayer(const std::shared_ptr<Player>& player);
public:
	Melee(ObjectImage& image, const Vector2& pos, const EnemyData& data) : Enemy(image, pos, data) {};
	void Move() override;
	void CheckAttackDelay() override;

	virtual EnemyType GetEnemyType() const override
	{
		return EnemyType::Melee;
	}
};

class Range : public Enemy {
private:
	const ObjectImage* bulletImage = nullptr;

	void SetPosDest();
	void Fire();
public:
	Range(ObjectImage& image, const Vector2& pos, const EnemyData& data) : Enemy(image, pos, data) {};
	void Move() override;
	void CheckAttackDelay() override;

	virtual EnemyType GetEnemyType() const override
	{
		return EnemyType::Range;
	}
};



class EnemyBullet;
struct BulletData;
class EnemyController {
private:
	int enemyID{};
	std::vector<Enemy*> enemies;

	std::shared_ptr<EnemyBullet> bullets{};

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

	const std::vector<Enemy*>& GetEnemies() const { return enemies; }
	const std::shared_ptr<EnemyBullet>& GetEnemyBullets() const { return bullets; }
};