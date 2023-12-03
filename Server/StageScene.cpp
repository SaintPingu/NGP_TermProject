#include "stdafx.h"
#include "StageScene.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ServerFramework.h"


void StageScene::StartBattle()
{
	std::cout << "플레이어 2인 입장 완료. 배틀 시작\n";
}

void StageScene::Init()
{

}

void StageScene::Update()
{

	// 배틀을 할 준비가 완료되면 각각의 클라이언트에게 GoBattle 명령어와 함께 팀원의 타입을 전달한다. ( 각각의 클라이언트의 타입은 이미 자신이 알고있다 )  
	//if (battleReady)
	//{
	//	std::unordered_map<int, std::shared_ptr<StagePlayer>>::iterator it = players.begin();
	//	std::shared_ptr<StagePlayer> P1 = it->second;	it++;
	//	std::shared_ptr<StagePlayer> P2 = it->second;

	//	P1->PushCommand(ServerStageCmd::GoBattle, P2->typeFly, P2->typeGnd);
	//	P2->PushCommand(ServerStageCmd::GoBattle, P1->typeFly, P1->typeGnd);

	//	/// +----------------------------------
	//	///			 씬 변경 ( 이벤트 )
	//	/// ----------------------------------+	
	//	int* P1_ID = new int(P1->ID);
	//	int* P2_ID = new int(P2->ID);
	//	SCENE_MGR->PushChangeLocationEvent(SceneEventType::ChangeClientLocation_ToBattle, &(P1_ID));
	//	SCENE_MGR->PushChangeLocationEvent(SceneEventType::ChangeClientLocation_ToBattle, &(P2_ID));

	//}
}

void StageScene::ProcessCommand(int clientID, Command command, void* data)
{
	auto& player = players.at(clientID);

	ClientStageCmd clientCmd = (ClientStageCmd)command;

	switch (clientCmd)
	{
	/// +----------------------------------
	///				캐릭터 선택  
	/// ----------------------------------+	
	case ClientStageCmd::EnterStage:
	{
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

		std::cout << "Batlle 대기열 입장 :: Client [" << player->ID << "], Fly=[" << (int)typeFly << "] Gnd=[" << (int)typeGnd << "]\n";

		// 두명의 클라이언트가 모두 캐릭터를 선택한 상황으로 battle을 할 준비가 완료됨  
		enterPlayers.insert(std::make_pair(player->ID, player));
		if (enterPlayers.size() == maxStagePlayer) {
			StartBattle();
		}

	}
		break;
	/// +----------------------------------
	///		 캐릭터 선택 이전으로 이동  
	/// ----------------------------------+	
	case ClientStageCmd::ExitStage:
	{
		/*
			캐릭터 선택하기 이전단계로 돌아간다. 
		*/
		player->typeFly = Type::Empty;
		player->typeGnd = Type::Empty;
		enterPlayers.erase(player->ID);
	}
		break;
	/// +----------------------------------
	///			    로비로 이동 
	/// ----------------------------------+	
	case ClientStageCmd::GoLobby:
	{
		SCENE_MGR->PushChangeLocationEvent(player->ID, SceneEventType::ChangeClientLocation_ToLobby);
	}
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
	// 팀원의 type 을 전달 ( 자신의 타입은 이미 클라이언트에서 알고 있음 )  
	// 상위 4비트 : fly Type
	// 하위 4비트 : gnd Type  
	BYTE packedTypeData = ((BYTE)other_fly << 4) | (BYTE)other_gnd;
	BYTE bCmd           = (BYTE)cmd;
	CLIENT_MGR->PushCommand(ID, bCmd, &packedTypeData, sizeof(BYTE));
}