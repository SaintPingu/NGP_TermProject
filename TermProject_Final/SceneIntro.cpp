#include "stdafx.h"
#include "SceneIntro.h"
#include "Framework.h"
#include "InputManager.h"

#define CLOUD_NUM 4
#define FIRSTCLOUD_X 125
#define FIRSTCLOUD_Y 187
#define SECONDCLOUD_X 187
#define SECONDCLOUD_Y 31
#define THIRDCLOUD_X -62
#define THIRDCLOUD_Y 93
#define FOURTHCLOUD_X 437
#define FOURTHCLOUD_Y 187

#define INSTRUCTION_BACKGROUND_X 500
#define INSTRUCTION_BACKGROUND_Y 750

#define LOGO_COLOR RGB(255, 212, 0)
#define LOGO_SHADOW_COLOR RGB(200, 100, 0)

#define LOGO_SIZE 106

#define MENU_SIZE 31

#define TIMERID_LODING 5
#define ELAPSE_LOADING 100

#define ALPHA 230

enum MI_Menu { start = 0, producer, finish };

void MainIntro::Init(const wchar_t* imgfile, float posX, float posY)
{
	img.Load(imgfile);

	size.x = img.GetWidth();
	size.y = img.GetHeight();

	// �̹����� ��ġ��
	pos.x = posX;
	pos.y = posY;

	// ���� �̹��� ��Ʈ��
	rectImage = { 0, 0, size.x, size.y };
}

void MainIntro::Render(HDC hdc)
{
	// ����� ���� ��ġ ��Ʈ��
	rectDraw = { pos.x, pos.y, size.x + pos.x, size.y + pos.y };

	// �̹��� ���
	img.TransparentBlt(hdc, rectDraw, rectImage, RGB(254, 254, 254));
}

// ���� ȭ�� ���� ������
void Cloud::Move(float MoveX, float MoveY)
{
	pos.x += MoveX * DeltaTime();
	pos.y += MoveY * DeltaTime();

	// ������ ��� �� ������ ���� ��ġ���� ����
	if (rectDraw.left > rectWindow.right)
		pos.x = -188;
}

void Logo::Render(HDC hdc)
{
	// ��Ʈ ����
	HFONT hFont = CreateFont(LOGO_SIZE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ChubbyChoo-SemiBold"));
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// ���� ���� ��������
	SetBkMode(hdc, TRANSPARENT);

	// �ΰ� �׸��� ���
	SetTextColor(hdc, LOGO_SHADOW_COLOR);
	TextOut(hdc, 45, 99 + logoMovingCnt, L"POKEMON", 7);
	TextOut(hdc, 99, 177 + logoMovingCnt, L"FLIGHT", 6);

	// �ΰ� ���
	SetTextColor(hdc, LOGO_COLOR);
	TextOut(hdc, 40, 94 + logoMovingCnt, L"POKEMON", 7);
	TextOut(hdc, 94, 172 + logoMovingCnt, L"FLIGHT", 6);

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

// ó�� ���� ȭ�� start, finish, finish�� ���� �޴� ����
void Menu::Render(HDC hdc)
{
	HFONT hFont = CreateFont(MENU_SIZE, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));

	switch (finger)
	{
	case MI_Menu::start:
		fingerPos.x = 175;
		fingerPos.y = 525;
		break;
	case MI_Menu::producer:
		fingerPos.x = 147;
		fingerPos.y = 575;
		break;
	case MI_Menu::finish:
		fingerPos.x = 165;
		fingerPos.y = 625;
		break;
	}

	TextOut(hdc, 200, 525, L"START", 5);
	TextOut(hdc, 174, 575, L"PRODUCER", 8);
	TextOut(hdc, 192, 625, L"FINISH", 6);

	if (isProducer)
	{
		background.AlphaBlend(hdc, 0, 0, 800, 1200, 0, 0, 800, 1200, ALPHA);

		HFONT hFont = CreateFont(31, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ChubbyChoo-SemiBold"));
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// ���� ���� ��������
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));


		TextOut(hdc, 50, 40, L"WINDOW PROGRAMMING", 19);
		TextOut(hdc, 50, 70, L"TERM PROJECT", 13);
		TextOut(hdc, 50, 160, L"PRODUCER", 9);
		TextOut(hdc, 50, 250, L"MIN DONG HYEON", 15);
		TextOut(hdc, 50, 340, L"BAEK SEUNG HO", 14);

		SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 50, 220, L"GAME PLAY", 10);
		SetTextColor(hdc, RGB(0, 255, 0));
		TextOut(hdc, 50, 310, L"GAME FLOW", 10);

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}

	// ȭ��ǥ�� �����Ÿ��� �κ� �������� 1.5�ʿ� �ѹ��� �����Ÿ�
	if (static_cast<int>(finger_twinkle) & 1 && !isProducer)
		TextOut(hdc, fingerPos.x, fingerPos.y, L"��", 1);

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void Menu::fingerController()
{
	if (KEY_TAP(VK_UP))
	{
		isProducer = false;
		if (--finger < 0)
		{
			finger = 0;
		}
	}
	else if (KEY_TAP(VK_DOWN))
	{
		isProducer = false;
		if (++finger > 2)
		{
			finger = 2;
		}
	}
	else if (KEY_TAP(VK_RETURN))
	{
		switch (finger)
		{
		case MI_Menu::start:
			SceneMgr->LoadScene(SceneType::Lobby);
			break;
		case MI_Menu::producer:
			isProducer = !isProducer;
			break;
		case MI_Menu::finish:
			PostQuitMessage(0);
			break;
		}
	}
	else if (KEY_TAP(VK_BACK) || KEY_TAP(VK_ESCAPE))
	{
		isProducer = false;
	}
}






void SceneIntro::MoveClouds()
{
	// �������� �ӵ� �ٸ��� ����
	clouds[0].Move(40, 0);
	clouds[1].Move(30, 0);
	clouds[2].Move(20, 0);
	clouds[3].Move(50, 0);
}

void SceneIntro::MoveLogo()
{
	static bool thiscnt = false;

	// �ΰ� �󸶳� ������������ Ȯ���� �� ���� ���� �ٽ� �ݴ�� �����̱�
	if (logo.logoMovingCnt > 10)
		thiscnt = true;
	else if (logo.logoMovingCnt < 0)
		thiscnt = false;

	if (thiscnt == true)
		logo.logoMovingCnt -= DeltaTime() * 10.f;
	else
		logo.logoMovingCnt += DeltaTime() * 10.f;
}

void SceneIntro::Init()
{
	clouds[0].Init(L"images\\intro\\Instruction_Cloud1.bmp", FIRSTCLOUD_X, FIRSTCLOUD_Y);
	clouds[1].Init(L"images\\intro\\Instruction_Cloud2.bmp", SECONDCLOUD_X, SECONDCLOUD_Y);
	clouds[2].Init(L"images\\intro\\Instruction_Cloud3.bmp", THIRDCLOUD_X, THIRDCLOUD_Y);
	clouds[3].Init(L"images\\intro\\Instruction_Cloud4.bmp", FOURTHCLOUD_X, FOURTHCLOUD_Y);
}

void SceneIntro::Render(HDC hdc)
{
	intro.Render(hdc);
	for (int i = 0; i < CLOUD_NUM; i++)
	{
		clouds[i].Render(hdc);
	}

	logo.Render(hdc);

	menu.Render(hdc);
}

void SceneIntro::Animate()
{
	menu.finger_twinkle += DeltaTime() * 2.f;
	MoveClouds();
	MoveLogo();
}

void SceneIntro::GetInput(CommandList* cmdList)
{
	menu.fingerController();
}

void SceneIntro::ProcessCommand()
{
	
}
