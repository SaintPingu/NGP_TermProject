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
		std::pair<SceneEventType, PVOID> sceneEvent = sceneEvents.front();
		sceneEvents.pop();


		switch (sceneEvent.first)
		{
		case SceneEventType::ChangeClientLocation_ToLobby:
		{
			int ID = *(int*)sceneEvent.second;
			SetClientLocation(ID, SceneType::Lobby);
			SAFE_DELETE(sceneEvent.second);
		}
			break;
		case SceneEventType::ChangeClientLocation_ToStage:
		{
			int ID = *(int*)sceneEvent.second;
			SetClientLocation(ID, SceneType::Stage);
			SAFE_DELETE(sceneEvent.second);

		}
			break;
		case SceneEventType::ChangeClientLocation_ToBattle:
		{
			int ID = *(int*)sceneEvent.second;
			SetClientLocation(ID, SceneType::Battle);
			SAFE_DELETE(sceneEvent.second);

		}
		break;
		}
	}

	return true;
}

void SceneMgr::EventPush(SceneEventType type, PVOID data)
{
	sceneEvents.push(std::make_pair(type, data));
}