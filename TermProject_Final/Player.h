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

public:
	Player(Type type, Type subType);
	~Player();
	void Render(HDC hdc);
	void RenderSkill(HDC hdc);

	void Animate(const HWND& hWnd);

	void ActiveSkill(Skill skill);
};