#pragma once
#include "Scene.h"
#include "Enemy.h"

class BattleMap
{
public:
	CImage _water;
	CImage _fire;
	CImage _electronic;
	CImage _dark;
	RECT _rectImage = { 0, };
	RECT _rectDraw = { 0, };
	RECT _rectDraw2 = { 0, };
	int _mapSpeed = 0;
	int _shakeCount = 0;
	int _shakeWindow = 0;
	bool _gameFinish = false;

	bool isShake = false;
	bool isShakeLeft = true;
	bool isShowHP = false;

public:
	BattleMap();
	void Init();
	void Render(HDC hdc, StageElement _select_index);

	inline constexpr void ShakeMap(int shakeCount = 5)
	{
		_shakeCount = shakeCount * 10;
		isShake = true;
	}
};

class Player;
class SceneBattle : public Scene {
private:
	// 12-03 최정일 Enemy 클래스 생성으로 주석 처리 
	struct EnemyData {
		Enemytype type{};
		Vector2 pos{};
		Dir dir{};
		bool isAction;
	};
	struct Bullet {
		BulletType type{};
		Vector2 pos{};
		Vector2 dir{};
	};
	struct Effect {
		EffectType type{};
		Vector2 pos{};
	};

	ObjectImage imgMelee;
	ObjectImage imgRange;

	Melee* meleeEnemy = nullptr; // 객체 하나씩만 생성해서 여러번 그리는 방식으로.
	Range* rangeEnemy = nullptr;

	std::unordered_map<BulletType, ObjectImage> bulletImages;

	StageElement stage;
	BattleMap battleMap;
	std::unordered_map<int, std::shared_ptr<Player>> players;
	std::vector<Bullet> bullets{};
	std::vector<EnemyData> enemies{};
	std::vector<Effect> effects{};


	void RenderPlayers(HDC hdc);
	void RenderBullets(HDC hdc);
	void RenderEffects(HDC hdc);
	void RenderEnemies(HDC hdc);
public:
	virtual void Init() override;
	virtual void Render(HDC hdc) override;
	virtual void Animate() override;
	virtual void GetInput(CommandList* cmdList) override;

	virtual void WriteData(void* data) override;
	virtual bool ProcessCommand() override;

	void CreatePlayer(int id, Type type, Type subType);
};
