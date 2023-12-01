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

	// 배틀을 할 준비가 완료되면 각각의 클라이언트에게 GoBattle 명령어와 함께 팀원의 타입을 전달한다. ( 각각의 클라이언트의 타입은 이미 자신이 알고있다 )  
	if (battleReady)
	{
		std::unordered_map<int, std::shared_ptr<StagePlayer>>::iterator it = players.begin();
		std::shared_ptr<StagePlayer> P1 = it->second;	it++;
		std::shared_ptr<StagePlayer> P2 = it->second;

		// StageScene 은 Cmd 만 전달을 하는데..
		// 팀원의 타입을 전달 - 이거 안하면 팀원 타입을 어디서 알아? 배틀씬에서? 
		// 배틀씬에서 넘기는 PlayerData 에 type 관련 데이터가 없는데..?
		P1->CommandPush(ServerStageCmd::GoBattle, P2->typeFly, P2->typeGnd);
		P2->CommandPush(ServerStageCmd::GoBattle, P1->typeFly, P1->typeGnd);

		/// +----------------------------------
		///			 씬 변경 ( 이벤트 )
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
	///		 해당 클라이언트 연결 종료  
	/// ----------------------------------+	
	case ClientStageCmd::Terminate:
	{
		// 해당 클라이언트와 연결 종료 코드 추가 필요.
		players.erase(clientID);
		curPlayerCnt -= 1;
		battleReady = false;
	}
		break;
	/// +----------------------------------
	///				캐릭터 선택  
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

		// 두명의 클라이언트가 모두 캐릭터를 선택한 상황으로 battle을 할 준비가 완료됨  
		if (curPlayerCnt == maxStagePlayer)
			battleReady = true;

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
		player.get()->typeFly = Type::Empty;
		player.get()->typeGnd = Type::Empty;

	}
		break;
	/// +----------------------------------
	///			    로비로 이동 
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
	// 2 명 이상은 못들어 오게 막는다. 
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
	// 팀원의 type 을 전달 ( 자신의 타입은 이미 클라이언트에서 알고 있음 )  
	// 상위 4비트 : fly Type
	// 하위 4비트 : gnd Type  
	BYTE packedTypeData = ((BYTE)other_fly << 4) | (BYTE)other_gnd;
	BYTE bCmd           = (BYTE)cmd;
	CLIENT_MGR->GetClient(ID)->GetCmdList()->CommandPush(bCmd, &packedTypeData, sizeof(BYTE));


}
