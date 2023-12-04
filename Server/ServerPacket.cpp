#include "stdafx.h"
#include "ClientMgr.h"
#include "ServerFramework.h"
#include "SceneMgr.h"
#include "LobbyScene.h"
#include "BattleScene.h"

//////////////////////////////////////////////////////////
#include "Player.h"
#include "Enemy.h"
//
//class Boss : public GameObject, public IMovable {
//	BossData* bossData;
//	EnemyBullet* bullets;
//	BossAct act;
//};
//
//class BattleScene {
//	Player players[2];
//	EnemyController* enemies;
//	Boss* boss;
//	EnemyBullet	enemyBullets;
//};

void PacketGenerator::GenerateData()
{ // GenerateData
	{// 로비 생성
		auto& lobby = SCENE_MGR->Lobby();
		auto& players = lobby->GetPlayers();

		Lobby::PlayerLobbyData* playerlobbydata = new Lobby::PlayerLobbyData[players.size()];
		int i{};
		for(const auto& [clientID, player] : players) {
			std::bitset<5> pid(clientID);
			std::bitset<1> mov(players.at(clientID)->isMoving);
			std::bitset<2> dir(static_cast<int>(players.at(clientID)->dir));
			std::bitset<8> byte(pid.to_string() + mov.to_string() + dir.to_string());

			playerlobbydata[i].Pid_Mov_Dir = static_cast<BYTE>(byte.to_ulong());
			playerlobbydata[i].Pos.x = players.at(clientID)->pos.x;
			playerlobbydata[i].Pos.y = players.at(clientID)->pos.y;
			++i;
		}
		

		lobbyData.PlayerCnt = players.size();
		lobbyData.PlayersData = playerlobbydata;
	}

	if(SCENE_MGR->GetGameData().isBattleStart)
	{// 배틀 생성 
		auto& battle = SCENE_MGR->Battle();
		auto& players = battle->GetPlayers();
		
		// PlayerBattleData //
		Battle::PlayerBattleData playerbattledata[2];
		int plCount{};
		for (auto& [playerID, player] : players) {
			playerbattledata[plCount].PlayerID = playerID;
			playerbattledata[plCount].Pos = player->GetPosCenter();
			++plCount;
		}
		battleData.PlayerBattleData[0] = playerbattledata[0];
		battleData.PlayerBattleData[1] = playerbattledata[1];

		// EnemyBattleData //
		Battle::EnemyBattleData enemybattledata{};
		auto enemies = battle->GetEnemyController()->GetEnemies();
		enemybattledata.EnemyCnt = enemies.size();
		enemybattledata.Enemies = new Battle::EnemyBattleData::Data[enemybattledata.EnemyCnt];
		for (int i = 0; i < enemybattledata.EnemyCnt; ++i) {
			std::bitset<2> type((BYTE)enemies[i]->GetEnemyType());
			std::bitset<3> dir((BYTE)enemies[i]->GetDir());
			std::bitset<1> action((BYTE)enemies[i]->IsAction()); //action?
			std::bitset<8> byte(type.to_string() + dir.to_string() + action.to_string());

			enemybattledata.Enemies[i].TypeDirActPad = static_cast<BYTE>(byte.to_ulong());
			enemybattledata.Enemies[i].Pos = enemies[i]->GetPosCenter();
		}
		battleData.EnemyData.EnemyCnt = enemybattledata.EnemyCnt;
		battleData.EnemyData.Enemies = enemybattledata.Enemies;
		return;

		Battle::BulletsBattleData bulletbattledata;
		auto& enemybullets = battle->GetEnemyController()->GetEnemyBullets()->GetBullets();
		auto& playerbullets = battle->GetPlayerController()->GetBullets();
		
		bulletbattledata.BulletCnt = enemybullets.size() + playerbullets.size();
		bulletbattledata.BulletsData = new Battle::BulletsBattleData::Data[bulletbattledata.BulletCnt];
		int bulletIdx{};
		// enemy bullet 
		for (const auto& bullet : enemybullets) {
			bulletbattledata.BulletsData[bulletIdx].bulletType =(BYTE)bullet->GetType();
			bulletbattledata.BulletsData[bulletIdx].Pos = bullet->GetPos();
			bulletbattledata.BulletsData[bulletIdx].Dir = bullet->GetBulletDirVector();
			++bulletIdx;
		}
		// 플레이어 bullet 
		for (const auto& bullet : playerbullets) {
			bulletbattledata.BulletsData[bulletIdx].bulletType = (BYTE)bullet->GetType();
			bulletbattledata.BulletsData[bulletIdx].Pos = bullet->GetPos();
			bulletbattledata.BulletsData[bulletIdx].Dir = bullet->GetBulletDirVector();
			++bulletIdx;
		}

		Battle::BossSkillBattleData bossskillbattledata{};
		//bossskillbattledata.EffectCnt = // 이펙트 어떤 데이터인지 잘 모르겠음..

		battleData.BulletData.BulletCnt = bulletbattledata.BulletCnt;
		battleData.BulletData.BulletsData = bulletbattledata.BulletsData;
		battleData.BossEffectData.EffectCnt = bossskillbattledata.EffectCnt;
		battleData.BossEffectData.Effects = bossskillbattledata.Effects;
	}
}

bool PacketGenerator::GeneratePacket(PacketBuffer& buffer, CommandList* cmdList, DataType type)
{
	BYTE cmdCnt = cmdList->GetCmdCnt();
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList를 비운다.

	buffer.clear();
	buffer.reserve(100);

	if (type == DataType::Lobby) {
		// 커맨드가 아무것도 없을 경우 None 전송
		if (pCommandList.empty()) {
			pCommandList.push_back((BYTE)ServerLobbyCmd::None);
		}

		// ServerLobbyCmd
		for (int i = 0;i < pCommandList.size();++i) {
			buffer.push_back(pCommandList[i]);
		}

		/* LobbyData */
		// PlayerCnt
		buffer.push_back(lobbyData.PlayerCnt);

		// PlayerLobbyData
		for (int i = 0; i < lobbyData.PlayerCnt; ++i) {
			constexpr int datalen = sizeof(Lobby::PlayerLobbyData);
			BYTE bytes[datalen];
			std::memcpy(bytes, &lobbyData.PlayersData[i], datalen);

			for (int j = 0; j < datalen; ++j) {
				buffer.push_back(bytes[j]);
			}
		}
	}
	else if (type == DataType::Stage) {
		if (pCommandList.empty()) {
			pCommandList.push_back((BYTE)ServerStageCmd::None);
		}

		for (int i = 0; i < pCommandList.size(); ++i) {
			buffer.push_back(pCommandList[i]); //ServerStageCmd
		}

	}
	else if (type == DataType::Battle) {

		// 테스트용
		if (pCommandList.empty()) {
			pCommandList.push_back((BYTE)ServerBattleCmd::None);
		}

		//커맨드리스트
		//buffer.push_back(cmdCnt); // cmdCnt
		for (int i = 0; i < pCommandList.size(); ++i) {
			buffer.push_back(pCommandList[i]); //ServerBattleCmd
		}
		
		{//PlayerBattleData[2] 
			BYTE bytes[sizeof(Battle::PlayerBattleData) * 2];
			std::memcpy(bytes, battleData.PlayerBattleData, sizeof(Battle::PlayerBattleData) * 2);
			for (int i = 0; i < sizeof(Battle::PlayerBattleData) * 2; ++i) {
				buffer.push_back(bytes[i]);
			}
		}


		{//Enemy개수 + (Battle::EnemyData * Enemy개수)
			buffer.push_back(battleData.EnemyData.EnemyCnt); //Enemy개수
			BYTE bytes[sizeof(Battle::EnemyBattleData::Data)];
			for (int i = 0; i < int(battleData.EnemyData.EnemyCnt); ++i) {

				memcpy(bytes, &battleData.EnemyData.Enemies[i], sizeof(Battle::EnemyBattleData::Data));

				for (int j = 0; j < sizeof(Battle::EnemyBattleData::Data); ++j) {
					buffer.push_back(bytes[j]);
				}
			}
		}
		return true;

		{//Bullet개수 + (Battle::BulletBattleData * Bullet개수)
			buffer.push_back(battleData.BulletData.BulletCnt); //Bullet개수
			BYTE bytes[sizeof(Battle::BulletsBattleData::Data)];
			for (int i = 0; i < int(battleData.BulletData.BulletCnt); ++i) {

				memcpy(bytes, &battleData.BulletData.BulletsData[i], sizeof(Battle::BulletsBattleData::Data));

				for (int j = 0; j < sizeof(Battle::BulletsBattleData::Data); ++j) {
					buffer.push_back(bytes[i]);
				}
			}
		}

		{//Effect개수 + (Effect * Effect개수) 
			buffer.push_back(battleData.BossEffectData.EffectCnt); //Effect개수
			BYTE bytes[sizeof(Battle::BossSkillBattleData::Data)];
			for (int i = 0; i < int(battleData.BossEffectData.EffectCnt); ++i) {

				memcpy(bytes, &battleData.BossEffectData.Effects[i], sizeof(Battle::BossSkillBattleData::Data));

				for (int j = 0; j < sizeof(Battle::BossSkillBattleData::Data); ++j) {
					buffer.push_back(bytes[i]);
				}
			}
		}
	}

	return true;
}

void PacketGenerator::DeleteData()
{
	delete[] lobbyData.PlayersData;
}

void PacketLoader::SetPacketBuffer(int clientID, std::vector<BYTE>* buffer)
{
	packetBuffers[clientID] = buffer;
	receivedPacketBuffers[clientID] = buffer;
}

int PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& data)
{
	constexpr int notAlloc = -1;
	if (packetBuffers.empty()) {
		return notAlloc;
	}

	if (crntClientID == notAlloc) {
		crntClientID = packetBuffers.begin()->first;
	}
	PacketBuffer* packetBuffer = packetBuffers[crntClientID];

	if (packetBuffer == nullptr || packetBuffer->empty()) {
		packetBuffers.erase(crntClientID);
		crntClientID = notAlloc;
		return -2; // buffer is empty
	}

	// 클라이언트가 보내는 패킷은 항상 commands다.
	// data는 항상 비운다.
	data.clear();
	SceneType type = SCENE_MGR->GetGameData().clientLocations[crntClientID];

	if (type == SceneType::Lobby || type == SceneType::Battle) {
		cmd = packetBuffer->front(); // 1byte
		packetBuffer->clear();
	}
	else if (type == SceneType::Stage) {
		cmd = packetBuffer->front(); // 1byte
		packetBuffer->erase(packetBuffer->begin());

		if (cmd == (BYTE)ClientStageCmd::EnterStage) {
			if (packetBuffer->empty()) {
				packetBuffers.erase(crntClientID);
				crntClientID = notAlloc;
				return -2;
			}
			data.push_back(packetBuffer->front()); // ClientStageData
			packetBuffer->erase(packetBuffer->begin());
		}
	}

	if (packetBuffer->empty()) {
		packetBuffers.erase(crntClientID);
		int clientID = crntClientID;
		crntClientID = notAlloc;
		return clientID;
	}

	return crntClientID;
}
