#include "stdafx.h"
#include "SceneMgr.h"
#include "LobbyScene.h"
//#include "StageScene.h"
//#include "BattleScene.h"

void SceneMgr::Init()
{
	lobbyScene = std::make_shared<LobbyScene>();
	//stageScene = std::make_shared<StageScene>();
	//battleScene = std::make_shared<BattleScene>();
}
