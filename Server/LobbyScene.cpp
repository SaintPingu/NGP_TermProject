#include "stdafx.h"
#include "LobbyScene.h"



void LobbyPlayer::Move()
{
	if (!isMoving) {
		return;
	}

}



void LobbyScene::Init()
{

}

void LobbyScene::ProcessCommand(int clientID, Command command, void* data)
{
	assert(players.count(clientID));
	auto& player = players[clientID];

	ClientLobbyCmd clientCmd = (ClientLobbyCmd)command;

	switch (clientCmd) {
	case ClientLobbyCmd::Terminate:
		// 해당 클라이언트와 연결 종료 코드 추가 필요.
		players.erase(clientID);
		return;
	case ClientLobbyCmd::MoveLeftTap:
		if (player->dir == Dir::Right) {
			player->dir = Dir::Empty;
			player->isMoving = false;
		}
		else {
			player->dir = Dir::Left;
			player->isMoving = true;
		}

		break;
	case ClientLobbyCmd::MoveRightTap:
		if (player->dir == Dir::Left) {
			player->dir = Dir::Empty;
			player->isMoving = false;
		}
		else {
			player->dir = Dir::Right;
			player->isMoving = true;
		}

		break;
	case ClientLobbyCmd::MoveUpTap:
		if (player->dir == Dir::Down) {
			player->dir = Dir::Empty;
			player->isMoving = false;
		}
		else {
			player->dir = Dir::Up;
			player->isMoving = true;
		}

		break;
	case ClientLobbyCmd::MoveDownTap:
		if (player->dir == Dir::Up) {
			player->dir = Dir::Empty;
			player->isMoving = false;
		}
		else {
			player->dir = Dir::Down;
			player->isMoving = true;
		}

		break;
	default:
		assert(0);
		break;
	}
}

// 하나의 키 입력으로 지속적으로 플레이어를 움직여야 한다.
void LobbyScene::Update()
{
	for (auto& [clientID, player] : players) {
		player->Move();
	}
}

