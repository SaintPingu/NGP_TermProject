#include "stdafx.h"
#include "LobbyScene.h"



void LobbyPlayer::Move()
{
	constexpr float speed = 200.f;

	if (!isMoving) {
		return;
	}

	int v{}, h{};

	switch (dir) {
	case Dir::Left:
		v = -1;
		break;
	case Dir::Right:
		v = 1;
		break;
	case Dir::Up:
		h = -1;
		break;
	case Dir::Down:
		h = 1;
		break;
	}

	if (v != 0) {
		pos.x += DeltaTime() * speed * v;

	}
	else if (h != 0) {
		pos.y += DeltaTime() * speed * h;
	}
}






void LobbyScene::Init()
{

}

void LobbyScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientLobbyCmd clientCmd = (ClientLobbyCmd)command;

	switch (clientCmd) {
	case ClientLobbyCmd::Terminate:
		// 해당 클라이언트와 연결 종료 코드 추가 필요.
		players.erase(clientID);
		return;
	case ClientLobbyCmd::MoveLeft:
		player->isMoving = true;
		player->dir = Dir::Left;
		break;
	case ClientLobbyCmd::MoveRight:
		player->isMoving = true;
		player->dir = Dir::Right;
		break;
	case ClientLobbyCmd::MoveUp:
		player->isMoving = true;
		player->dir = Dir::Up;
		break;
	case ClientLobbyCmd::MoveDown:
		player->isMoving = true;
		player->dir = Dir::Down;
		break;
	case ClientLobbyCmd::Stop:
		player->isMoving = false;
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

