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
		// �ش� Ŭ���̾�Ʈ�� ���� ���� �ڵ� �߰� �ʿ�.
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

// �ϳ��� Ű �Է����� ���������� �÷��̾ �������� �Ѵ�.
void LobbyScene::Update()
{
	for (auto& [clientID, player] : players) {
		player->Move();
	}
}

