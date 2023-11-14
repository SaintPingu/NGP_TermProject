#include "stdafx.h"
#include "LobbyScene.h"


void LobbyScene::Init()
{

}

void LobbyScene::ProcessCommand(int clientID, Command command, void* data)
{
	ClientLobbyCmd clientCmd = (ClientLobbyCmd)command;

	switch (clientCmd) {
	case ClientLobbyCmd::Terminate:
		break;
	case ClientLobbyCmd::MoveLeft:
		break;
	case ClientLobbyCmd::MoveRight:
		break;
	case ClientLobbyCmd::MoveUp:
		break;
	case ClientLobbyCmd::MoveDown:
		break;
	default:
		assert(0);
		break;
	}
}