#include "stdafx.h"
#include "StageScene.h"

void StageScene::Init()
{

}

void StageScene::Update()
{

}

void StageScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientStageCmd clientCmd = (ClientStageCmd)command;

	switch (clientCmd)
	{
	/// +----------------------------------
	///		 �ش� Ŭ���̾�Ʈ ���� ����  
	/// ----------------------------------+	
	case ClientStageCmd::Terminate:
	{
		// �ش� Ŭ���̾�Ʈ�� ���� ���� �ڵ� �߰� �ʿ�.
		players.erase(clientID);
		curPlayerCnt -= 1;
		battleReady = false;
	}
		break;
	/// +----------------------------------
	///				ĳ���� ����  
	/// ----------------------------------+	
	case ClientStageCmd::EnterStage:
	{

		std::bitset<8> type = *(int*)data;
		std::bitset<4> fly(type.to_string().substr(0, 4));
		std::bitset<4> gnd(type.to_string().substr(4, 4));

		Type typeFly = static_cast<Type>(fly.to_ulong());
		Type typeGnd = static_cast<Type>(gnd.to_ulong());

		player.get()->typeFly = typeFly;
		player.get()->typeGnd = typeGnd;

		// �θ��� Ŭ���̾�Ʈ�� ��� ĳ���͸� ������ ��Ȳ���� battle�� �� �غ� �Ϸ��  
		if (curPlayerCnt == maxStagePlayer)
			battleReady = true;

	}
		break;
	/// +----------------------------------
	///		 ĳ���� ���� �������� �̵�  
	/// ----------------------------------+	
	case ClientStageCmd::ExitStage:
	{
		/*
			ĳ���� �����ϱ� �����ܰ�� ���ư���. 
		*/
		player.get()->typeFly = Type::Empty;
		player.get()->typeGnd = Type::Empty;

	}
		break;
	/// +----------------------------------
	///			    �κ�� �̵� 
	/// ----------------------------------+	
	case ClientStageCmd::GoLobby:
	{

		players.erase(clientID);
		curPlayerCnt -= 1;
		battleReady   = false;

	}
		break;

	}

}

bool StageScene::AddPlayer(int clientID)
{
	// 2 �� �̻��� ����� ���� ���´�. 
	if (curPlayerCnt < maxStagePlayer)
		curPlayerCnt += 1;
	else
		return false;

	players[clientID] = std::make_shared<StagePlayer>();
	return true;
}
