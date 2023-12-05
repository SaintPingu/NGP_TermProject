#pragma once
#include "Object.h"


class Player : public GameObject, public IAnimatable {
private:
	Type type = Type::Empty;
	Type subType = Type::Empty;

	/*PlayerBullet* bullets = nullptr;
	PlayerBullet* subBullets = nullptr;*/
	Vector2 posDest = { 0, };
	Vector2 vectorMove = { 0, };
	float alpha = 0;

	//SkillManager* skillManager = nullptr;
	int deathFrame = 40;

	Pokemon pokemon = Pokemon::Null;
	SubPokemon subPokemon = SubPokemon::Null;
	ObjectImage img_subPokemon{};

	bool isDeath;

	float hp = 50, maxHp = 50;
	float mp = 0, maxMp = 50;

public:
	Player(Type type, Type subType);
	~Player();
	void Render(HDC hdc);
	void RenderSkill(HDC hdc);

	void Animate(const HWND& hWnd);

	void ActiveSkill(Skill skill);

	
	void SetDeath(bool val) { isDeath = val; }
	bool IsDeath() { return isDeath; }
	Type GetType() { return type; }
	Type GetSubType() { return subType; }


	float GetHP() {
		return hp;
	}
	void SetHp(float val) {
		hp = val;
	}
	float GetMaxHP() {
		return maxHp;
	}
	float GetMP() {
		return mp;
	}
	void SetMp(float val) {
		mp = val;
	}
	float GetMaxMP() {
		return maxMp;
	}
};