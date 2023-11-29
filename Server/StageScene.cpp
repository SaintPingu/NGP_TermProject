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
	return true;
}
