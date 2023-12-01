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


void SceneMgr::UpdateScenes()
{
	lobbyScene->Update();
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
	// Event Ã³¸® 
	int ID = *(int*)data;
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
	}
	SAFE_DELETE(data);
}

void SceneMgr::PushChangeLocationEvent(SceneEventType type, PVOID data)
{
	// Event ¸¦ Push  
	sceneEvents.push(std::make_pair(type, data));
}