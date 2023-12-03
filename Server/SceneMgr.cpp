#include "stdafx.h"
#include "SceneMgr.h"
#include "LobbyScene.h"
#include "StageScene.h"
#include "BattleScene.h"

void SceneMgr::Init()
{
	lobbyScene  = std::make_shared<LobbyScene>();
	stageScene  = std::make_shared<StageScene>();
	battleScene = std::make_shared<BattleScene>();
}

void SceneMgr::InsertClient(int id)
{
	gameData.clientLocations[id] = SceneType::Lobby;
	lobbyScene->AddClient(id);
}
void SceneMgr::DeleteClient(int id)
{
	if (!gameData.clientLocations.count(id)) {
		return;
	}

	SceneType crntClientLocation = gameData.clientLocations[id];

	switch (crntClientLocation) {
	case SceneType::Lobby:
		lobbyScene->RemoveClient(id);
		break;
	case SceneType::Stage:
		stageScene->RemoveClient(id);
		break;
	case SceneType::Battle:
		battleScene->RemoveClient(id);
		break;
	default:
		break;
	}
	gameData.clientLocations.erase(id);
}

void SceneMgr::SetClientLocation(int id, SceneType type)
{
	SceneType crntClientLocation = gameData.clientLocations[id];

	std::string befLoc{};
	switch (crntClientLocation) {
	case SceneType::Lobby:
		lobbyScene->RemoveClient(id);
		befLoc = "Lobby";
		break;
	case SceneType::Stage:
		stageScene->RemoveClient(id);
		befLoc = "Stage";
		break;
	case SceneType::Battle:
		battleScene->RemoveClient(id);
		befLoc = "Battle";
		break;
	default:
		assert(0);
		break;
	}

	std::string aftLoc{};
	switch (type) {
	case SceneType::Lobby:
		lobbyScene->AddClient(id);
		aftLoc = "Lobby";
		break;
	case SceneType::Stage:
		stageScene->AddClient(id);
		aftLoc = "Stage";
		break;
	case SceneType::Battle:
		battleScene->AddClient(id);
		aftLoc = "Battle";
		break;
	default:
		assert(0);
		break;
	}

	// 테스트용 출력
	std::cout << "Client[" << id << "] 씬 위치 변경[" << befLoc << "] ==> [" << aftLoc << "]\n";

	gameData.clientLocations[id] = type;
}


void SceneMgr::UpdateScenes()
{
	lobbyScene->Update();
	stageScene->Update();
}

bool SceneMgr::Event()
{
	while (!sceneEvents.empty())
	{
		// first : SceneType / Second : Data
		std::pair<SceneEventType, PVOID> sceneEvent = sceneEvents.front();
		sceneEvents.pop();


		switch (sceneEvent.first)
		{
		case SceneEventType::ChangeClientLocation_ToLobby:
		case SceneEventType::ChangeClientLocation_ToStage:
		case SceneEventType::ChangeClientLocation_ToBattle:
		{
			ChangeLocationEvent(sceneEvent.first, sceneEvent.second);
		}
		break;
		}
	}

	return true;
}

void SceneMgr::ChangeLocationEvent(SceneEventType type, PVOID data)
{
	// Event 처리 
	int ID = *(static_cast<int*>(data));
	switch (type) {
	case SceneEventType::ChangeClientLocation_ToLobby: 
	{
		SetClientLocation(ID, SceneType::Lobby);
	}
		break;
	case SceneEventType::ChangeClientLocation_ToStage:
	{
		SetClientLocation(ID, SceneType::Stage);
	}
		break;
	case SceneEventType::ChangeClientLocation_ToBattle:
	{
		SetClientLocation(ID, SceneType::Battle);
	}
		break;
	default:
		assert(0);
		break;
	}

	int* int_data = static_cast<int*>(data);
	SAFE_DELETE(int_data);
}

void SceneMgr::PushChangeLocationEvent(int clientID, SceneEventType type)
{
	// Event 를 Push  
	int* data = new int(clientID);
	sceneEvents.push(std::make_pair(type, PVOID(data)));
}
