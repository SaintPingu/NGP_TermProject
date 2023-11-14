#include "stdafx.h"
#include "Scene.h"

LobbyScene::LobbyScene()
{

}

LobbyScene::~LobbyScene()
{
}



void LobbyScene::Init()
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

	RECT rectWindow = { 0,0,500 ,750 };

	
	rectDraw = rectWindow;

	rectImage = rectWindow;

	playerdata.rectImage = { 0, 0, TPLAYER_IMAGESIZE_X, TPLAYER_IMAGESIZE_Y };

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

	StopPlayer();

	// 절대 좌표값 출력을 위한 변수
	//playerdata.aboutMapPos = { playerdata.Pos.x, playerdata.Pos.y };

}

void LobbyScene::Render(HDC hdc)
{

}

void LobbyScene::Animate()
{

}

void LobbyScene::StopPlayer()
{
	playerdata.rectImage.left = 0; // 정지시 차렷자세
}

int LobbyScene::GetCamSizeX()
{
	return CAMSIZE_X;
}
int LobbyScene::GetCamSizeY()
{
	return CAMSIZE_Y;
}