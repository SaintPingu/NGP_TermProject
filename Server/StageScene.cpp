#include "stdafx.h"
#include "StageScene.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ServerFramework.h"


void StageScene::Init()
{

}

void StageScene::Update()
{

	// ��Ʋ�� �� �غ� �Ϸ�Ǹ� ������ Ŭ���̾�Ʈ���� GoBattle ��ɾ�� �Բ� ������ Ÿ���� �����Ѵ�. ( ������ Ŭ���̾�Ʈ�� Ÿ���� �̹� �ڽ��� �˰��ִ� )  
	if (battleReady)
	{
		std::unordered_map<int, std::shared_ptr<StagePlayer>>::iterator it = players.begin();
		std::shared_ptr<StagePlayer> P1 = it->second;	it++;
		std::shared_ptr<StagePlayer> P2 = it->second;

		// StageScene �� Cmd �� ������ �ϴµ�..
		// ������ Ÿ���� ���� - �̰� ���ϸ� ���� Ÿ���� ��� �˾�? ��Ʋ������? 
		// ��Ʋ������ �ѱ�� PlayerData �� type ���� �����Ͱ� ���µ�..?
		P1->CommandPush(ServerStageCmd::GoBattle, P2->typeFly, P2->typeGnd);
		P2->CommandPush(ServerStageCmd::GoBattle, P1->typeFly, P1->typeGnd);

		/// +----------------------------------
		///			 �� ���� ( �̺�Ʈ )
		/// ----------------------------------+	
		int* P1_ID = new int(P1->ID);
		int* P2_ID = new int(P2->ID);
		SCENE_MGR->EventPush(SceneEventType::ChangeClientLocation_ToBattle, &(P1_ID));
		SCENE_MGR->EventPush(SceneEventType::ChangeClientLocation_ToBattle, &(P2_ID));

	}
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
	players[clientID].get()->ID = clientID;
	return true;
}

void StageScene::Clear()
{
	battleReady  = false;
	curPlayerCnt = 0;
	players.clear();
}

void StagePlayer::CommandPush(ServerStageCmd cmd, Type other_fly, Type other_gnd)
{
	// ������ type �� ���� ( �ڽ��� Ÿ���� �̹� Ŭ���̾�Ʈ���� �˰� ���� )  
	// ���� 4��Ʈ : fly Type
	// ���� 4��Ʈ : gnd Type  
	BYTE packedTypeData = ((BYTE)other_fly << 4) | (BYTE)other_gnd;
	BYTE bCmd           = (BYTE)cmd;
	CLIENT_MGR->GetClient(ID)->GetCmdList()->CommandPush(bCmd, &packedTypeData, sizeof(BYTE));


}
