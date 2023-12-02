#pragma once
#include "Scene.h"

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
	struct Eenemy {
		Type type{};
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

	std::unordered_map<BulletType, ObjectImage> bulletImages;

	StageElement stage;
	BattleMap battleMap;
	std::unordered_map<int, std::shared_ptr<Player>> players;
	std::vector<Bullet> bullets{};
	std::vector<Eenemy> enemies{};
	std::vector<Effect> effects{};

	void RenderPlayers(HDC hdc);
	void RenderBullets(HDC hdc);
	void RenderEffects(HDC hdc);
public:
	virtual void Init() override;
	virtual void Render(HDC hdc) override;
	virtual void Animate() override;
	virtual void GetInput(CommandList* cmdList) override;

	virtual void WriteData(void* data) override;
	virtual bool ProcessCommand() override;

	void CreatePlayer(int id, Type type, Type subType);
};
