#include "stdafx.h"
#include "SceneLobby.h"
#include "Framework.h"
#include "SceneManager.h"
#include "InputManager.h"

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
	playerImage.Load(L"images\\town\\TownPlayer.png");
	npc[0].image.Load(L"images\\town\\npc1.bmp");
	npc[1].image.Load(L"images\\town\\npc2.png");
	npc[2].image.Load(L"images\\town\\npc3.bmp");
	npc[3].image.Load(L"images\\town\\npc4.bmp");
	exits.Load(L"images\\dialog\\Exit.bmp");
	glowing_black.Load(L"images\\loading\\Loading_Black_background.bmp");

	npc[0].Init(75, 40, 50, 27);
	npc[1].Init(40, 40, 64, 64);
	npc[2].Init(24, 30, 38, 48);
	npc[3].Init(34, 36, 54, 58);

	npc[0].SetPosition({ 125, 515 });
	npc[1].SetPosition({ 406, 219 });
	npc[2].SetPosition({ 581, 402 });
	npc[3].SetPosition({ 550, 175 });

	npc[0].SetTransparent();
	npc[2].SetTransparent();
	npc[3].SetTransparent();

	RECT rectBackground = { 0,0,500 ,750 };

	rectImage = rectBackground;
	rectDraw = rectBackground;
	
	lobbyPlayers[0] = LobbyPlayer(Vector2(100, 100), Dir::Down, false);
}

void SceneLobby::Render(HDC hdc)
{
	LobbyPlayer myPlayer = lobbyPlayers[framework->client_ID];
	int camLeft = myPlayer.pos.x - CAMSIZE_X;
	int camRight = myPlayer.pos.x + CAMSIZE_X;

	RECT rectBackground = rectImage;
	
	if (camLeft < 0) {
		camLeft = 0;
	}
	if (camLeft > 250) {
		camLeft = 250;
	}

	rectBackground.left += camLeft;
	rectBackground.right += camLeft;
	background.Draw(hdc, rectDraw, rectBackground);

	npc[0].Draw(hdc, camLeft);
	npc[1].Draw(hdc, camLeft);
	npc[2].Draw(hdc, camLeft);
	npc[3].Draw(hdc, camLeft);

	for (auto& [id, player] : lobbyPlayers) {
		playerImage.Draw(hdc, player.pos.x - 20, player.pos.y - 20, 40, 40,
			player.rectImage.left, player.rectImage.top, player.rectImage.right, player.rectImage.bottom);
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
	// 테스트를 위한 임시값
	if (KEY_PRESSED(VK_RIGHT)) {
		lobbyPlayers[framework->client_ID].pos.x += DeltaTime() * 100;
	}
	if (KEY_PRESSED(VK_LEFT)) {
		lobbyPlayers[framework->client_ID].pos.x -= DeltaTime() * 100;
	}
	//

	NpcAnimate();
	PlayerAnimate();
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
	static float npc1cnt = 0;
	static float npc2cnt = 0;

	// npc2의 방향을 반대로 바꾸기 위한 변수
	static int negative_number = 1;

	// npc1의 모션 주기
	npc1cnt+= DeltaTime() * 30;
	// npc2의 모션 주기
	npc2cnt+= DeltaTime() * 120;

	// npc1의 애니메이션
	if ((int)npc1cnt > 5 && npc[0].drawPos.x < 150)
	{
		npc1cnt = 0;
		npc[0].drawPos.x += 50;
	}
	else if(npc[0].drawPos.x >= 150) {
		if (npc1cnt > 50) {
			npc1cnt = 0;
			npc[0].drawPos.x = 0;
		}
	}

	// npc2가 위치값이 1000이 넘을 경우 반대로 다시 걸어가기
	if (npc[1].pos.y > 625)
	{
		negative_number = -1;
		npc[1].drawPos.y = 192;
	}
	if (npc[1].pos.y < 219)
	{
		negative_number = 1;
		npc[1].drawPos.y = 0;
	}

	if ((int)npc2cnt >= 20)
	{
		npc2cnt = 0;
		npc[1].drawPos.x += 64;
		npc[1].pos.y += 6 * negative_number;
		if (npc[1].drawPos.x > 192) {
			npc[1].drawPos.x = 0;
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
	for (auto& [id, player] : lobbyPlayers) {
		if (!player.isMoving) {
			StopPlayer(id);
			continue;
		}

		player.rectImage.left += TPLAYER_IMAGESIZE_X;

		if (player.rectImage.left == TPLAYER_MAX_IMAGESIZE_X)
			player.rectImage.left = 0;

		switch (player.dir)
		{
		case Dir::Left:
			player.rectImage.top = DIR_LEFT_IMAGE_Y;
			break;
		case Dir::Up:
			player.rectImage.top = DIR_UP_IMAGE_Y;
			break;
		case Dir::Right:
			player.rectImage.top = DIR_RIGHT_IMAGE_Y;
			break;
		case Dir::Down:
			player.rectImage.top = DIR_DOWN_IMAGE_Y;
			break;
		default:
			break;
		}
	}
}

void SceneLobby::StopPlayer(int id)
{
	lobbyPlayers[id].rectImage.left = 0; // 정지시 차렷자세
	lobbyPlayers[id].isMoving = false; // 키 업시 움직임 모션 타이머 정지
}

void NPC::Init(int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
	this->srcWidth = srcWidth;
	this->srcHeight = srcHeight;
	this->dstWidth = dstWidth;
	this->dstHeight = dstHeight;
}

void NPC::SetTransparent()
{
	isTransparent = true;
}

void NPC::Draw(HDC hdc, int camX)
{
	if (isTransparent) {
		image.TransparentBlt(hdc, pos.x - camX, pos.y, srcWidth, srcHeight, drawPos.x, drawPos.y, dstWidth, dstHeight, RGB(255, 255, 255));
	}
	else {
		image.Draw(hdc, pos.x - camX, pos.y, srcWidth, srcHeight, drawPos.x, drawPos.y, dstWidth, dstHeight);
	}

}
