#pragma once

// 2023-11-13 (�ε���) : GameData, SceneMgr �⺻ Ʋ �ۼ�

class LobbyScene;
class StageScene;
class BattleScene;


class SceneMgr {
	struct GameData {
		bool isBattleStart{ false };
		StageElement crntStageType{ StageElement::Water };
		std::unordered_map<int, SceneType> clientLocations{};
	};

private:
	GameData gameData;
	std::shared_ptr<LobbyScene> lobbyScene{};

	//std::shared_ptr<StageScene> stageScene{};
	//std::shared_ptr<BattleScene> battleScene{};

public:
	void Init();
	void InsertClient(int id) { gameData.clientLocations[id] = SceneType::Lobby; }

	const std::shared_ptr<LobbyScene>& Lobby() const { return lobbyScene; }
	//const std::shared_ptr<StageScene>& Stage() const { return stageScene; }
	//const std::shared_ptr<BattleScene>& Battle() const { return battleScene; }

	GameData& GetGameData() { return gameData; }
	SceneType GetClientLocation(int id) const { return gameData.clientLocations.at(id); }
};