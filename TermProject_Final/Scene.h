#pragma once
#include "stdafx.h"
#include "Image.h"

#define TPLAYER_IMAGESIZE_X 64
#define TPLAYER_IMAGESIZE_Y 64

#define CAMSIZE_X 162
#define CAMSIZE_Y 375

class Scene {
public:
	virtual void Init() abstract;
	virtual void Render(HDC hdc) abstract;
	virtual void Animate() abstract;
};

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

class LobbyScene : Scene {
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

	RECT building[18];
	CImage background;
	NPC npc[4];


	Player playerdata;
	CImage exits;
	CImage glowing_black;
	bool exit = false;

	RECT rectImage = { 0, };
	RECT rectDraw = { 0, };

public:
	LobbyScene();
	~LobbyScene();

	virtual void Init();
	virtual void Render(HDC hdc);
	virtual void Animate();

	void StopPlayer();

	int GetCamSizeX();
	int GetCamSizeY();
};