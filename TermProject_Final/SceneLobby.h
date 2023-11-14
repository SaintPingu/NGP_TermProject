#pragma once
#include "stdafx.h"
#include "Scene.h"

#define TPLAYER_IMAGESIZE_X 64
#define TPLAYER_IMAGESIZE_Y 64

#define CAMSIZE_X 162
#define CAMSIZE_Y 375

#define ALPHA 150

#define TOWN_OBJECT_NUM 18

struct LobbyPlayer {
	Vector2 pos;
	Dir dir;
	bool isMoving;
};

struct NPC {
	CImage image;
	Vector2 pos;
	RECT rect;
};

class SceneLobby : public Scene {
	class Player
	{
	public:
		POINT aboutMapPos;
		CImage img;
		//POINT pos = { 0, };
		RECT rectDraw = { 0, };
		RECT rectImage = { 0, };
		RECT cam = { 0, };
		//Dir dir = Dir::Down;
		//bool keepGoing = false;
	};
private:
	std::map<int, LobbyPlayer> lobbyPlayers;

	RECT building[18]; // 서버에서 충돌체크 하므로 필요 없는거 아닌가?
	CImage background;
	NPC npc[4];


	Player playerdata;
	CImage exits;
	CImage glowing_black;
	bool lobbyexit = false;

	RECT rectImage = { 0, };
	RECT rectDraw = { 0, };

public:
	bool _allHide = false;

	SceneLobby();
	~SceneLobby();

	virtual void Init();
	virtual void Render(HDC hdc);
	virtual void Animate();

	int GetCamSizeX();
	int GetCamSizeY();
};