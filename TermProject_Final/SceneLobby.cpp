#include "stdafx.h"
#include "SceneLobby.h"

SceneLobby::SceneLobby()
{

}

SceneLobby::~SceneLobby()
{

}

void SceneLobby::Init()
{
	playerdata = Player();
	background.Load(L"images\\town\\Town.png");
	playerdata.img.Load(L"images\\town\\TownPlayer.png");
	npc[0].image.Load(L"images\\town\\npc1.bmp");
	npc[1].image.Load(L"images\\town\\npc2.png");
	npc[2].image.Load(L"images\\town\\npc3.bmp");
	npc[3].image.Load(L"images\\town\\npc4.bmp");
	exits.Load(L"images\\dialog\\Exit.bmp");
	glowing_black.Load(L"images\\loading\\Loading_Black_background.bmp");

	// 왼쪽 상단부터
	building[0] = { 0, 0, 125, 190 };
	building[1] = { 156, 0, 300, 100 };
	building[2] = { 337, 0, 750, 175 };
	building[3] = { 644, 175, 750, 300 };
	building[4] = { 0, 253, 144, 750 };
	building[5] = { 130, 556, 267, 750 };
	building[6] = { 267, 594, 325, 750 };
	building[7] = { 325, 619, 362, 750 };
	building[8] = { 687, 487, 750, 750 };
	building[9] = { 725, 300, 750, 372 };
	building[10] = { 218, 290, 247, 312 };
	building[11] = { 306, 290, 334, 312 };
	building[12] = { 184, 394, 312, 481 };
	building[13] = { 500, 312, 600, 406 };
	building[14] = { 184, 325, 187, 481 };
	building[15] = { 506, 500, 594, 594 };
	building[16] = { 0, 710, 750, 750 };
	building[17] = { 189, 80, 191, 100 };

	npc[0].rect = { 125, 515, 194, 556 };
	npc[1].rect = { 406, 219, 194, 556 };
	npc[2].rect = { 581, 402, 605, 432 };
	npc[3].rect = { 550, 175, 387, 424 };

	RECT rectWindow = { 0,0,500 ,750 }; // 모르겠어서 임의로 넣음 원래는 인자로 넘어왔었음.

	rectImage = rectWindow;
	rectDraw = rectWindow;

	playerdata.rectImage = { 0, 0, TPLAYER_IMAGESIZE_X, TPLAYER_IMAGESIZE_Y };

	/*lobbyPlayers[0] = LobbyPlayer(Vector2(100, 100), Dir::Down, false);
	lobbyPlayers[1] = LobbyPlayer(Vector2(200, 100), Dir::Down, false);
	lobbyPlayers[2] = LobbyPlayer(Vector2(300, 100), Dir::Down, false);
	lobbyPlayers[3] = LobbyPlayer(Vector2(400, 100), Dir::Down, false);
	lobbyPlayers[4] = LobbyPlayer(Vector2(500, 100), Dir::Down, false);*/

	/*if (sceneManager->GetPrevScene() == Scene::Intro)
	{
		playerdata->_Pos.x = 60;
		playerdata->_Pos.y = 232;
		playerdata->_dir = Dir::Right;
	}
	else if (sceneManager->GetPrevScene() == Scene::Stage)
	{
		playerdata->_Pos.x = rectWindow.right / 2;
		playerdata->_Pos.y = rectWindow.bottom / 2;
		playerdata->_dir = Dir::Down;
	}*/

	//StopPlayer();

	// 절대 좌표값 출력을 위한 변수
	//playerdata.aboutMapPos = { playerdata.Pos.x, playerdata.Pos.y };

}

void SceneLobby::Render(HDC hdc)
{

	background.Draw(hdc, rectDraw, rectImage);

	npc[1].image.Draw(hdc, npc[1].rect.left, npc[1].rect.top, 40, 40, npc[1].pos.x, npc[1].pos.y, 64, 64);

	npc[2].image.TransparentBlt(hdc, npc[2].rect.left, npc[2].rect.top, 24, 30, 0, 0, 38, 48, RGB(255, 255, 255));

	npc[3].image.TransparentBlt(hdc, npc[3].rect.left, npc[3].rect.top, 34, 36, 0, 0, 54, 58, RGB(255, 255, 255));

	for (auto& lplayer : lobbyPlayers) {
		playerdata.img.Draw(hdc, lplayer.second.pos.x - 20, lplayer.second.pos.y - 20, 40, 40,
			playerdata.rectImage.left, playerdata.rectImage.top, playerdata.rectImage.right, playerdata.rectImage.bottom);
	}

	npc[0].image.TransparentBlt(hdc, npc[0].rect.left, npc[0].rect.top, 75, 40, npc[0].pos.x, npc[0].pos.y, 50, 27, RGB(255, 255, 255));

	if (_allHide == true)
	{
		// 건물 오브젝트1 박스
		TCHAR lpOut[TOWN_OBJECT_NUM];
		for (int i = 0; i < TOWN_OBJECT_NUM; i++)
		{
			wsprintf(lpOut, L"%d", i);
			Rectangle(hdc, building[i].left, building[i].top, building[i].right, building[i].bottom);
			TextOut(hdc, building[i].left + 10, building[i].top + 10, lpOut, lstrlen(lpOut));
		}


		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, playerdata.rectDraw.left, playerdata.rectDraw.top, playerdata.rectDraw.right, playerdata.rectDraw.bottom);
		Rectangle(hdc, playerdata.cam.left, playerdata.cam.top, playerdata.cam.right, playerdata.cam.bottom);

		TCHAR Posx[1000];
		TCHAR Posy[1000];
		/*wsprintf(Posx, L"X : %d", player->aboutMapPos.x);
		wsprintf(Posy, L"Y : %d", player->aboutMapPos.y);
		TextOut(hdc, playerdata..x - 25, playerdata.Pos.y + 30, Posx, lstrlen(Posx));
		TextOut(hdc, playerdata.Pos.x - 25, playerdata.Pos.y + 50, Posy, lstrlen(Posy));*/

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}

	if (lobbyexit == true)
	{
		glowing_black.AlphaBlend(hdc, 0, 0, 500, 750, 0, 0, 500, 750, ALPHA);
		exits.Draw(hdc, rectImage.right / 2 - 100, rectImage.bottom / 2 + 60, 200, 51, 0, 0, 810, 210);

		HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ChubbyChoo-SemiBold"));
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		TextOut(hdc, 70, rectImage.bottom / 2, L"PRESS ENTER KEY TO EXIT...", 25);

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);
	}
}

void SceneLobby::Animate()
{

}

int SceneLobby::GetCamSizeX()
{
	return CAMSIZE_X;
}
int SceneLobby::GetCamSizeY()
{
	return CAMSIZE_Y;
}