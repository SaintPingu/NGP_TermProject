#include "stdafx.h"
#include "SceneLobby.h"
#include "Framework.h"
#include "SceneManager.h"

#define TPLAYER_SPEED 4;

SceneLobby::SceneLobby()
{

}

SceneLobby::~SceneLobby()
{

}

void SceneLobby::Init()
{
	background.Load(L"images\\town\\Town.png");
	masterPlayerData.Load(L"images\\town\\TownPlayer.png");
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

	RECT rect = { 0,0,500 ,750 }; // 모르겠어서 임의로 넣음 원래는 인자로 넘어왔었음.

	rectImage = rect;
	rectDraw = rect;

	playerdata.push_back(Player());
	playerdata[0].rectImage = {0, 0, TPLAYER_IMAGESIZE_X, TPLAYER_IMAGESIZE_Y};
	
	lobbyPlayers[0] = LobbyPlayer(Vector2(100, 100), Dir::Down, false);
	lobbyPlayers[1] = LobbyPlayer(Vector2(200, 100), Dir::Up, false);
	lobbyPlayers[2] = LobbyPlayer(Vector2(300, 100), Dir::Left, false);
	lobbyPlayers[3] = LobbyPlayer(Vector2(400, 100), Dir::Right, false);
	lobbyPlayers[4] = LobbyPlayer(Vector2(100, 300), Dir::Down, false);

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
		if (lplayer.first >= playerdata.size()) {
			break;
		}
		masterPlayerData.Draw(hdc, lplayer.second.pos.x - 20, lplayer.second.pos.y - 20, 40, 40,
			playerdata[lplayer.first].rectImage.left, playerdata[lplayer.first].rectImage.top, playerdata[lplayer.first].rectImage.right, playerdata[lplayer.first].rectImage.bottom);
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

		for (int i = 0;i < lobbyPlayers.size();++i) {
			Rectangle(hdc, playerdata[i].rectDraw.left, playerdata[i].rectDraw.top, playerdata[i].rectDraw.right, playerdata[i].rectDraw.bottom);
			Rectangle(hdc, playerdata[i].cam.left, playerdata[i].cam.top, playerdata[i].cam.right, playerdata[i].cam.bottom);
		}
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

	if (playerdata.size() > 1) {
		//playerdata.erase(playerdata.begin() + 1, playerdata.end());
	}
}

void SceneLobby::Animate()
{
	RECT temp;

	if (lobbyPlayers.size() != playerdata.size()) {
		for (int i = 1; i < lobbyPlayers.size();++i) {
			playerdata.push_back(Player());
			playerdata[i].rectImage = { 0, 0, TPLAYER_IMAGESIZE_X, TPLAYER_IMAGESIZE_Y };
		}
	}

	for (int i = 0;i < lobbyPlayers.size();++i) {
		Vector2 playerPos = lobbyPlayers[i].pos;
		playerdata[i].cam = {int(playerPos.x) - CAMSIZE_X, rectWindow.top, int(playerPos.x) + CAMSIZE_X, rectWindow.bottom};
	}

	NpcAnimate();

	if (GetAsyncKeyState(VK_RETURN) & 0x8000 && lobbyexit == true)
	{
		PostQuitMessage(0);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		lobbyPlayers[framework->client_ID].isMoving = true;
		lobbyPlayers[framework->client_ID].dir = Dir::Left;

		playerdata[framework->client_ID].aboutMapPos.x -= TPLAYER_SPEED;
		lobbyexit = false;

		if (playerdata[framework->client_ID].cam.left < rectWindow.left && rectImage.left > 0)
		{
			rectImage.right -= TPLAYER_SPEED;
			rectImage.left -= TPLAYER_SPEED;

			// 모든 건물들을 자리 그대로 놓기 -> 안하면 플레이어 이동시 오브젝트가 같이 움직임
			for (int i = 0; i < TOWN_OBJECT_NUM; i++)
			{
				building[i].left += TPLAYER_SPEED;
				building[i].right += TPLAYER_SPEED;
			}

			for (int i = 0; i < 4; ++i) {
				npc[i].rect.left += TPLAYER_SPEED;
			}
		}
		else {
			lobbyPlayers[framework->client_ID].pos.x -= TPLAYER_SPEED;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		lobbyPlayers[framework->client_ID].isMoving = true;
		lobbyPlayers[framework->client_ID].dir = Dir::Right;
		playerdata[framework->client_ID].aboutMapPos.x += TPLAYER_SPEED;
		lobbyexit = false;

		if (playerdata[framework->client_ID].cam.right > rectWindow.right && rectImage.right < 748)
		{
			rectImage.right += TPLAYER_SPEED;
			rectImage.left += TPLAYER_SPEED;

			for (int i = 0; i < TOWN_OBJECT_NUM; i++)
			{
				building[i].left -= TPLAYER_SPEED;
				building[i].right -= TPLAYER_SPEED;
			}
			for (int i = 0; i < 4; ++i) {
				npc[i].rect.left -= TPLAYER_SPEED;
			}
		}
		else {
			lobbyPlayers[framework->client_ID].pos.x += TPLAYER_SPEED;
		}
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		playerdata[framework->client_ID].aboutMapPos.y -= TPLAYER_SPEED;
		lobbyPlayers[framework->client_ID].isMoving = true;
		lobbyPlayers[framework->client_ID].dir = Dir::Up;
		lobbyPlayers[framework->client_ID].pos.y -= TPLAYER_SPEED;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		lobbyexit = false;
		playerdata[framework->client_ID].aboutMapPos.y += TPLAYER_SPEED;
		lobbyPlayers[framework->client_ID].isMoving = true;
		lobbyPlayers[framework->client_ID].dir = Dir::Down;
		lobbyPlayers[framework->client_ID].pos.y += TPLAYER_SPEED;
	}

	PlayerAnimate();

}

void SceneLobby::WriteData(void* data)
{
	PacketBuffer* buffer = static_cast<PacketBuffer*>(data);

	Lobby::LobbyData lobbydata;
	lobbydata.PlayersData = new Lobby::PlayerLobbyData[*buffer->begin()];
	memcpy(&lobbydata, &(*buffer->begin()), sizeof(*buffer));

	for (int i = 0 ; i < lobbydata.PlayerCnt; ++i) {
		Lobby::PlayerLobbyData* pldata = &lobbydata.PlayersData[i];
		//[ playerID(5) , isMoving(1) , dir(2) ]=> 1byte
		int playerID = pldata->Pid_Mov_Dir >> 3; 
		pldata->Pid_Mov_Dir = (pldata->Pid_Mov_Dir << 5) >> 5;

		bool isMoving = pldata->Pid_Mov_Dir >> 2;
		pldata->Pid_Mov_Dir = (pldata->Pid_Mov_Dir << 6) >> 6;

		int dir = pldata->Pid_Mov_Dir;

		lobbyPlayers[playerID].pos = pldata->Pos;
		lobbyPlayers[playerID].isMoving = isMoving;

		// +1 인 이유는 원래 empty 포함 0~4 표현인데 2비트에는 0~3까지 밖에 표현을 못하므로..
		lobbyPlayers[playerID].dir = (Dir)(dir + 1); 
	}
}

int SceneLobby::GetCamSizeX()
{
	return CAMSIZE_X;
}
int SceneLobby::GetCamSizeY()
{
	return CAMSIZE_Y;
}

void SceneLobby::NpcAnimate()
{
	std::shared_ptr<Scene> scene = framework->GetSeceneMgr()->GetCurrentScene();
	static int npc1cnt = 0;
	static int npc2cnt = 0;

	// npc2의 방향을 반대로 바꾸기 위한 변수
	static int negative_number = 1;

	//if (scene == Scene::Town)
	{
		// npc1의 모션 주기
		npc1cnt++;
		// npc2의 모션 주기
		npc2cnt++;

		// npc1의 애니메이션
		if (npc[0].pos.x < 150)
			npc[0].pos.x += 50;
		if (npc1cnt == 100)
		{
			npc1cnt = 0;
			npc[0].pos.x = 0;
		}

		// npc2가 위치값이 1000이 넘을 경우 반대로 다시 걸어가기
		if (npc[1].rect.top > 625)
		{
			negative_number = -1;
			npc[1].pos.y = 192;
		}
		if (npc[1].rect.top < 219)
		{
			negative_number = 1;
			npc[1].pos.y = 0;
		}

		// warking npc2의 cnt가 5의 배수가 될때마다 앞으로 한걸음
		if (npc2cnt % 5 == 0)
		{
			npc[1].pos.x += 64;
			npc[1].rect.top += 6 * negative_number;
		}
		if (npc2cnt == 20 && npc[1].pos.x > 192)
		{
			npc2cnt = 0;
			npc[1].pos.x = 0;
		}
	}
}


#define TPLAYER_IMAGESIZE_X 64
#define TPLAYER_MAX_IMAGESIZE_X 256

#define DIR_LEFT_IMAGE_Y 64
#define DIR_RIGHT_IMAGE_Y 128
#define DIR_UP_IMAGE_Y 192
#define DIR_DOWN_IMAGE_Y 0


void SceneLobby::PlayerAnimate()
{

	for (int i = 0;i < playerdata.size();++i) {
		playerdata[i].rectImage.left += TPLAYER_IMAGESIZE_X;

		if (playerdata[i].rectImage.left == TPLAYER_MAX_IMAGESIZE_X)
			playerdata[i].rectImage.left = 0;

		switch (lobbyPlayers[i].dir)
		{
		case Dir::Left:
			playerdata[i].rectImage.top = DIR_LEFT_IMAGE_Y;
			break;
		case Dir::Up:
			playerdata[i].rectImage.top = DIR_UP_IMAGE_Y;
			break;
		case Dir::Right:
			playerdata[i].rectImage.top = DIR_RIGHT_IMAGE_Y;
			break;
		case Dir::Down:
			playerdata[i].rectImage.top = DIR_DOWN_IMAGE_Y;
			break;
		default:
			break;
		}
		if (!lobbyPlayers[i].isMoving) {
			StopPlayer(i);
		}
	}
}

void SceneLobby::StopPlayer(int idx)
{
	playerdata[idx].rectImage.left = 0; // 정지시 차렷자세
	lobbyPlayers[idx].isMoving = false; // 키 업시 움직임 모션 타이머 정지
}
