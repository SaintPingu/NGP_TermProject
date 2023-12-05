#include "stdafx.h"
#include "StageScene.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ServerFramework.h"
#include "BattleScene.h"


void StageScene::StartBattle()
{
	if (players.size() < 2) {
		return;
	}

	std::cout << "�÷��̾� 2�� ���� �Ϸ�. ��Ʋ ����\n";

	/*BYTE crntStageType = (BYTE)SCENE_MGR->GetCrntStageType();
	CLIENT_MGR->PushCommand(clientID, (BYTE)ServerLobbyCmd::GoStage, &crntStageType, sizeof(BYTE));
	SCENE_MGR->PushChangeLocationEvent(clientID, SceneEventType::ChangeClientLocation_ToStage);*/

	std::unordered_map<int, std::shared_ptr<StagePlayer>>::iterator it = players.begin();
	std::shared_ptr<StagePlayer> P1 = it->second;	it++;
	std::shared_ptr<StagePlayer> P2 = it->second;

	P1->PushCommand(ServerStageCmd::GoBattle, P2->typeFly, P2->typeGnd);
	P2->PushCommand(ServerStageCmd::GoBattle, P1->typeFly, P1->typeGnd);

	/// +----------------------------------
	///			 �� ���� ( �̺�Ʈ )
	/// ----------------------------------+	
	SCENE_MGR->PushChangeLocationEvent(P1->ID, SceneEventType::ChangeClientLocation_ToBattle);
	SCENE_MGR->PushChangeLocationEvent(P2->ID, SceneEventType::ChangeClientLocation_ToBattle);
	CLIENT_MGR->ClearCommand(P1->ID);
	CLIENT_MGR->ClearCommand(P2->ID);

	BattleStart(P1, P2);
	players.erase(P1->ID);
	players.erase(P2->ID);
	enterPlayers.clear();
}

void StageScene::Init()
{

}

void StageScene::Update()
{

	// ��Ʋ�� �� �غ� �Ϸ�Ǹ� ������ Ŭ���̾�Ʈ���� GoBattle ��ɾ�� �Բ� ������ Ÿ���� �����Ѵ�. ( ������ Ŭ���̾�Ʈ�� Ÿ���� �̹� �ڽ��� �˰��ִ� )  
	//if (battleReady)
	//{
	//	std::unordered_map<int, std::shared_ptr<StagePlayer>>::iterator it = players.begin();
	//	std::shared_ptr<StagePlayer> P1 = it->second;	it++;
	//	std::shared_ptr<StagePlayer> P2 = it->second;

	//	P1->PushCommand(ServerStageCmd::GoBattle, P2->typeFly, P2->typeGnd);
	//	P2->PushCommand(ServerStageCmd::GoBattle, P1->typeFly, P1->typeGnd);

	//	/// +----------------------------------
	//	///			 �� ���� ( �̺�Ʈ )
	//	/// ----------------------------------+	
	//	int* P1_ID = new int(P1->ID);
	//	int* P2_ID = new int(P2->ID);
	//	SCENE_MGR->PushChangeLocationEvent(SceneEventType::ChangeClientLocation_ToBattle, &(P1_ID));
	//	SCENE_MGR->PushChangeLocationEvent(SceneEventType::ChangeClientLocation_ToBattle, &(P2_ID));

	//}
}

void StageScene::ProcessCommand(int clientID, Command command, void* data)
{
	if (!players.count(clientID)) {
		return;
	}

	auto& player = players.at(clientID);

	ClientStageCmd clientCmd = (ClientStageCmd)command;

	switch (clientCmd)
	{
	case ClientStageCmd::None:
		break;
	/// +----------------------------------
	///				ĳ���� ����  
	/// ----------------------------------+	
	case ClientStageCmd::EnterStage:
	{
		if (IsBattleStart()) {
			break;
		}
		PacketBuffer* buffer = static_cast<PacketBuffer*>(data);
		Stage::ClientStageData clientStageData = (Stage::ClientStageData)buffer->front();
		buffer->clear();

		std::bitset<8> type ((BYTE)clientStageData.Fly_Gnd);
		std::bitset<4> fly(type.to_string().substr(0, 4));
		std::bitset<4> gnd(type.to_string().substr(4, 4));

		Type typeFly = static_cast<Type>(fly.to_ulong());
		Type typeGnd = static_cast<Type>(gnd.to_ulong());

		player->typeFly = typeFly;
		player->typeGnd = typeGnd;

		std::cout << "Batlle ��⿭ ���� :: Client [" << player->ID << "], Fly=[" << (int)typeFly << "] Gnd=[" << (int)typeGnd << "]\n";

		// �θ��� Ŭ���̾�Ʈ�� ��� ĳ���͸� ������ ��Ȳ���� battle�� �� �غ� �Ϸ��  
		enterPlayers.insert(std::make_pair(player->ID, player));
		if (enterPlayers.size() == maxStagePlayer) {
			StartBattle();
		}

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
		std::cout << "Batlle ��⿭ ���� ��� :: Client [" << player->ID << "]\n";
		player->typeFly = Type::Empty;
		player->typeGnd = Type::Empty;
		enterPlayers.erase(player->ID);
	}
		break;
	/// +----------------------------------
	///			    �κ�� �̵� 
	/// ----------------------------------+	
	case ClientStageCmd::GoLobby:
	{
		SCENE_MGR->PushChangeLocationEvent(player->ID, SceneEventType::ChangeClientLocation_ToLobby);
	}
		break;
	default:
		break;
	}

}

void StageScene::AddClient(int clientID)
{
	players[clientID] = std::make_shared<StagePlayer>();
	players[clientID]->ID = clientID;
}

void StageScene::RemoveClient(int clientID)
{
	players.erase(clientID);
	enterPlayers.erase(clientID);
}


void StageScene::Clear()
{
	//battleReady  = false;
	enterPlayers.clear();
	players.clear();
}

void StagePlayer::PushCommand(ServerStageCmd cmd, Type other_fly, Type other_gnd)
{
	// ������ type �� ���� ( �ڽ��� Ÿ���� �̹� Ŭ���̾�Ʈ���� �˰� ���� )  
	// ���� 4��Ʈ : fly Type
	// ���� 4��Ʈ : gnd Type  
	BYTE packedTypeData = ((BYTE)other_fly << 4) | (BYTE)other_gnd;
	BYTE bCmd           = (BYTE)cmd;

	std::cout << "[" << ID << "] - " << (int)bCmd << std::endl;

	CLIENT_MGR->PushCommand(ID, bCmd, &packedTypeData, sizeof(BYTE));
}