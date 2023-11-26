#pragma once
#include "Scene.h"

class Battle
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
	Battle();
	void Init();
	void Render(HDC hdc, StageElement _select_index);

	inline constexpr void ShakeMap(int shakeCount = 5)
	{
		_shakeCount = shakeCount * 10;
		isShake = true;
	}
};

class SceneBattle : public Scene {
private:
	StageElement stage;
	Battle battle;

public:
	virtual void Init() override;
	virtual void Render(HDC hdc) override;
	virtual void Animate() override;
	virtual void GetInput(CommandList* cmdList) override;

	virtual void WriteData(void* data) override;
	virtual void ProcessCommand() override;
};
