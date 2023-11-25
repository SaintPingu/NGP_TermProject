#include "stdafx.h"
#include "LobbyScene.h"



void LobbyPlayer::Move()
{
	constexpr float speed = 200.f;
	if (v != 0) {
		pos.x += DeltaTime() * speed * v;

	}
	else if (h != 0) {
		pos.y += DeltaTime() * speed * h;
	}
}

bool LobbyPlayer::IsMoving()
{
	if (v == 0 && h == 0) {
		return false;
	}
	return true;
}

Dir LobbyPlayer::GetDir()
{
	switch (v) {
	case -1:
		return Dir::Left;
	case 1:
		return Dir::Right;
	}

	switch (h) {
	case -1:
		return Dir::Up;
	case 1:
		return Dir::Down;
	}

	return befDir;
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
		player->v = -1;
		player->h = 0;
		player->befDir = Dir::Left;
		break;
	case ClientLobbyCmd::MoveRight:
		player->v = 1;
		player->h = 0;
		player->befDir = Dir::Right;
		break;
	case ClientLobbyCmd::MoveUp:
		player->v = 0;
		player->h = -1;
		player->befDir = Dir::Up;
		break;
	case ClientLobbyCmd::MoveDown:
		player->v = 0;
		player->h = 1;
		player->befDir = Dir::Down;
		break;
	case ClientLobbyCmd::Stop:
		player->v = 0;
		player->h = 0;
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

