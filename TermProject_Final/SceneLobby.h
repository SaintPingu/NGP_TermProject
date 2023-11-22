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
	Vector2 pos{};
	Dir dir{};
	bool isMoving{};
	RECT rectImage{ 0, 0, TPLAYER_IMAGESIZE_X, TPLAYER_IMAGESIZE_Y };
};

class NPC {
private:
	int srcWidth{};
	int srcHeight{};
	int dstWidth{};
	int dstHeight{};
public:
	bool isTransparent{};
	CImage image;
	Vector2 pos;
	Vector2 drawPos;

	void Init(int srcWidth, int srcHeight, int dstWidth, int dstHeight);
	void SetPosition(Vector2 pos) { this->pos = pos; }
	void SetTransparent();
	void Draw(HDC hdc, int camX);
};

class SceneLobby : public Scene {
	struct PlayerData
	{
	public:
		CImage img;
	};
private:
	std::map<int, LobbyPlayer> lobbyPlayers;

	CImage background;
	NPC npc[4];

	CImage playerImage;

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


	void NpcAnimate();
	void PlayerAnimate();
	void StopPlayer(int id);
};