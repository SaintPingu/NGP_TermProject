#include "stdafx.h"
#include "SceneIntro.h"
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

#define MENUsize 31

#define TIMERID_LODING 5
#define ELAPSE_LOADING 100

#define ALPHA 230

enum MI_Menu { start = 0, producer, finish };

void MainIntro::Init(const wchar_t* imgfile, int posX, int posY)
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

void MainIntro::Paint(HDC hdc, const RECT& rectWindow)
{
	// ����� ���� ��ġ ��Ʈ��
	rectDraw = { pos.x, pos.y, size.x + pos.x, size.y + pos.y };

	// �̹��� ���
	img.TransparentBlt(hdc, rectDraw, rectImage, RGB(254, 254, 254));
}

// ���� ȭ�� ���� ������
void Cloud::Move(int MoveX, int MoveY, const RECT& rectWindow)
{
	pos.x += MoveX;
	pos.y += MoveY;

	// ������ ��� �� ������ ���� ��ġ���� ����
	if (rectDraw.left > rectWindow.right)
		pos.x = -188;
}

void Logo::Paint(HDC hdc)
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
void Menu::Paint(HDC hdc)
{
	HFONT hFont = CreateFont(MENUsize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));
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
	if (finger_twinkle_cnt % 3 != 0 && !isProducer)
		TextOut(hdc, fingerPos.x, fingerPos.y, L"��", 1);

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}

void Menu::fingerController(const HWND& hWnd)
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
			//sceneManager->StartLoading(hWnd);
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
	//const RECT rectWindow = sceneManager->GetRectDisplay();
	RECT rectWindow = { 0, 0, WINDOWSIZE_X , WINDOWSIZE_Y };
	// �������� �ӵ� �ٸ��� ����
	clouds[0].Move(4, 0, rectWindow);
	clouds[1].Move(2, 0, rectWindow);
	clouds[2].Move(1, 0, rectWindow);
	clouds[3].Move(4, 0, rectWindow);
}

void SceneIntro::MoveLogo()
{
	static bool thiscnt = false;

	// �ΰ� �󸶳� ������������ Ȯ���� �� ���� ���� �ٽ� �ݴ�� �����̱�
	if (logo.logoMovingCnt > 10)
		thiscnt = true;
	else if (logo.logoMovingCnt == 0)
		thiscnt = false;

	if (thiscnt == true)
		logo.logoMovingCnt--;
	else
		logo.logoMovingCnt++;
}

void SceneIntro::Init()
{
	clouds[0].Init(L"images\\intro\\Instruction_Cloud1.bmp", FIRSTCLOUD_X, FIRSTCLOUD_Y);
	clouds[1].Init(L"images\\intro\\Instruction_Cloud2.bmp", SECONDCLOUD_X, SECONDCLOUD_Y);
	clouds[2].Init(L"images\\intro\\Instruction_Cloud3.bmp", THIRDCLOUD_X, THIRDCLOUD_Y);
	clouds[3].Init(L"images\\intro\\Instruction_Cloud4.bmp", FOURTHCLOUD_X, FOURTHCLOUD_Y);
}

void SceneIntro::Render(HDC hdc, const RECT& rectWindow)
{
	intro.Paint(hdc, rectWindow);
	for (int i = 0; i < CLOUD_NUM; i++)
	{
		clouds[i].Paint(hdc, rectWindow);
	}

	logo.Paint(hdc);

	menu.Paint(hdc);
}

void SceneIntro::Animate()
{
	menu.finger_twinkle_cnt++;
	MoveClouds();
	MoveLogo();
}
