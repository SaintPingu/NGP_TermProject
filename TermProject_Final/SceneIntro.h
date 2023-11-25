#pragma once
#include "Scene.h"

class MainIntro
{
public:
	Vector2 pos = { 0, };
	FRECT rectDraw = { 0, };

	void Init(const wchar_t* imgfile, float posX, float posY);
	virtual void Paint(HDC hdc);

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
	void Move(float MoveX, float MoveY);
};

class Logo : public MainIntro
{
public:
	float logoMovingCnt = 0;
public:
	void Paint(HDC hdc);
};

class Menu : public MainIntro
{
public:
	CImage background;
	int finger = 0;
	float finger_twinkle = 0;
	int finger_difficulty = 0;
	bool isProducer = false;
	POINT fingerPos = { 0, };
public:
	Menu()
	{
		background.Load(L"images\\loading\\Loading_Black_background.bmp");
	}
	void Paint(HDC hdc);
	void fingerController();
};



class SceneIntro : public Scene {
	Intro intro{};
	Logo logo{};
	Menu menu{};
	Cloud clouds[4];
	int finger = 0;
	bool isProducer = false;

	void MoveClouds();
	void MoveLogo();
public:
	virtual void Init() override;
	virtual void Render(HDC hdc) override;
	virtual void Animate() override;
	virtual void GetInput(CommandList* cmdList);
	virtual void ProcessCommand();

	virtual void WriteData(void* data) override {}
};
