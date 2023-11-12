#pragma once
#include "stdafx.h"
#include "Image.h"

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
	//Image image; � �̹���?
	Vector2 pos;
};

class LobbyScene : Scene {
private:
	std::map<int, LobbyPlayer> lobbyPlayers;

	RECT building[18];
	//Image background; � �̹���?
	NPC npc[4];
public:
	LobbyScene();
	~LobbyScene();

	virtual void Init();
	virtual void Render(HDC hdc);
	virtual void Animate();
};