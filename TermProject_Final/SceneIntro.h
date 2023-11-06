#pragma once
#include "Scene.h"

class MainIntro
{
public:
	POINT pos = { 0, };
	RECT rectDraw = { 0, };

	void Init(const wchar_t* imgfile, int posX, int posY);
	virtual void Paint(HDC hdc, const RECT& rectWindow);

private:
	CImage img;
	POINT size = { 0, };
	RECT rectImage = { 0, };
};

class Intro : public MainIntro
{
public:
	Intro()
	{
		Init(L"images\\intro\\Instruction_Background2.png", 0, 0);
	}
};

class Cloud : public MainIntro
{
public:
	void Move(int MoveX, int MoveY, const RECT& rectWindow);
};

class Logo : public MainIntro
{
public:
	int logoMovingCnt = 0;
public:
	void Paint(HDC hdc);
};

class Menu : public MainIntro
{
public:
	CImage background;
	int finger = 0;
	int finger_twinkle_cnt = 0;
	int finger_difficulty = 0;
	bool isProducer = false;
	POINT fingerPos = { 0, };
public:
	Menu()
	{
		background.Load(L"images\\loading\\Loading_Black_background.bmp");
	}
	void Paint(HDC hdc, HWND hWnd);
	void fingerController(const HWND& hWnd);
};



class SceneIntro : public Scene {
	Intro intro{};
	Logo logo{};
	Menu menu{};
	Cloud clouds[4];

public:
	virtual void Init() override;
	virtual void Render(HDC hdc, const RECT& rectWindow) override;
	virtual void Animate() override;
};
