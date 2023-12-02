#include "stdafx.h"
#include "LobbyScene.h"
#include "DataBase.h"
#include "ClientMgr.h"

#define TPLAYER_IMAGESIZE_X 32
#define TPLAYER_IMAGESIZE_Y 32

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

RECT LobbyPlayer::GetRect()
{
	RECT rect{};
	rect.left = pos.x - TPLAYER_IMAGESIZE_X / 2;
	rect.right = rect.left + TPLAYER_IMAGESIZE_X;
	rect.top = pos.y - TPLAYER_IMAGESIZE_Y / 2;
	rect.bottom = rect.top + TPLAYER_IMAGESIZE_Y;
	
	return rect;
}




bool LobbyScene::CheckMoveScene(const std::shared_ptr<LobbyPlayer>& player)
{
	if (player->pos.x + 20 >= rectWindow.right) {
		return true;
	}
	else if (player->pos.x - 20 <= rectWindow.left) {
		int clientID = player->GetID();
		CLIENT_MGR->PushCommand(clientID, (BYTE)ServerLobbyCmd::GoMenu, nullptr, 0);
		CLIENT_MGR->RegisterTerminateClientID(clientID);
		RemovePlayer(clientID);
		return true;
	}

	return false;
}

void LobbyScene::Init()
{

}

void LobbyScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = GetPlayer(clientID);
	if (!player) {
		return;
	}

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

void LobbyScene::AddPlayer(int clientID)
{
	players[clientID] = std::make_shared<LobbyPlayer>();
	players[clientID]->SetID(clientID);
}

bool LobbyScene::CheckCollision(RECT playerRect)
{
	RECT temp;
	for (int i = 0; i < 18; ++i) {
		RECT rect = DATABASE->buildingLocations[i];
		if (IntersectRect(&temp, &playerRect, &rect))
		{
			return true;
		}
	}

	return false;
}

const std::shared_ptr<LobbyPlayer>& LobbyScene::GetPlayer(int id)
{
	if (players.count(id)) {
		return players[id];
	}

	std::cout << "[ERROR - LobbyScene_GetPlayer()] 플레이어 ID [" << id << "] 가 존재하지 않습니다!\n";
	return nullptr;
}

// 하나의 키 입력으로 지속적으로 플레이어를 움직여야 한다.
void LobbyScene::Update()
{
	for (auto& [clientID, player] : players) {
		player->befpos = player->pos;
		player->Move();
		if (CheckMoveScene(player)) {
			continue;
		}
		else if (CheckCollision(player->GetRect())) {
			player->pos = player->befpos;
		}
	}
}

void LobbyScene::RemovePlayer(int clientID)
{
	players.erase(clientID);
}
