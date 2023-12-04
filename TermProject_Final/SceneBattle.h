#pragma once
#include "Scene.h"
#include "Boss.h"
#include "GUIManager.h"

class Enemy;
class Melee;
class Range;

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
	class EnemyData {
	public:
		EnemyType type{};
		Vector2 pos{};
		Dir dir{};
		bool isAction;
		float frame{};

		void Render(HDC hdc, std::shared_ptr<Enemy> enemy);
	};
	struct Bullet {
		BulletType type{};
		POINT pos{};
		Vector2 dir{};
	};
	struct Effect {
		EffectType type{};
		Vector2 pos{};
	};


	ObjectImage imgMelee;
	ObjectImage imgRange;

	std::shared_ptr<Melee> meleeEnemy{}; // 객체 하나씩만 생성해서 여러번 그리는 방식으로.
	std::shared_ptr<Range> rangeEnemy{};
	std::shared_ptr<Boss> boss{};
	std::shared_ptr<GUIManager> gui{};

	std::unordered_map<BulletType, ObjectImage> bulletImages;

	StageElement stage;
	BattleMap battleMap;
	std::unordered_map<int, std::shared_ptr<Player>> players;
	std::vector<Bullet> bullets{};
	std::unordered_map<int, EnemyData> enemies{};
	std::vector<Effect> effects{};


	void RenderPlayers(HDC hdc);
	void RenderBullets(HDC hdc);
	void RenderEffects(HDC hdc);
	void RenderEnemies(HDC hdc);


	void AnimatePlayers();

	int v{}, h{};

public:
	virtual void Init() override;
	virtual void Render(HDC hdc) override;
	virtual void Animate() override;
	virtual void GetInput(CommandList* cmdList) override;

	virtual void WriteData(void* data) override;
	virtual bool ProcessCommand() override;

	void CreatePlayer(int id, Type type, Type subType);
	const std::shared_ptr<Player>& GetPlayer(int id) const { return players.at(id); }
};

void SetMyPlayer(Type fly, Type gnd);
void SetOtherPlayer(Type fly, Type gnd);
Type GetPlayerFlyType();
Type GetPlayerGndType();