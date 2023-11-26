#include "stdafx.h"
#include "ClientMgr.h"
#include "ServerFramework.h"

#include "SceneMgr.h"
#include "LobbyScene.h"

//////////////////////////////////////////////////////////

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

	{// 배틀 생성 -- 일정 미뤄야할듯.

	}
}

void PacketGenerator::GeneratePacket(PacketBuffer& buffer, CommandList* cmdList, DataType type)
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList를 비운다.

	buffer.clear();
	buffer.reserve(100);

	if (type == DataType::Lobby) {
		// 데이터 길이 = 커맨드리스트 길이 + 플레이어 개수 + (플레이어 개수 * 플레이어 데이터)
		int len = pCommandList.size() + sizeof(BYTE) + (lobbyData.PlayerCnt * sizeof(Lobby::PlayerLobbyData));

		// Datalen
		BYTE lenBytes[sizeof(int)];
		std::memcpy(lenBytes, &len, sizeof(int));
		for (int i = 0; i < sizeof(int); ++i) {
			buffer.push_back(lenBytes[i]);
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

			for (int i = 0; i < datalen; ++i) {
				buffer.push_back(bytes[i]);
			}
		}
	}
	else if (type == DataType::Stage) {
		BYTE len = pCommandList.size();
		buffer.push_back(len); // Datalen

		for (int i = 0; i < pCommandList.size(); ++i) {
			buffer.push_back(pCommandList[i]); //ServerStageCmd
		}

	}
	else if (type == DataType::Battle) {
		int len = pCommandList.size() + sizeof(battleData); // battle씬은 int 4바이트의 datalen을 보낸다.
		void* vlen = &len;
		for (int i = 0; i < sizeof(int); ++i) {
			buffer.push_back(((BYTE*)vlen)[i]); // Datalen	
		}

		buffer.push_back(pCommandList.size()); // cmdCnt

		for (int i = 0; i < pCommandList.size(); ++i) {
			buffer.push_back(pCommandList[i]); //ServerBattleCmd
		}

		BYTE bytes[sizeof(battleData)];
		std::memcpy(bytes, &battleData, sizeof(battleData));

		for (int i = 0; i < sizeof(battleData); ++i) {
			buffer.push_back(bytes[i]); //battleData
		}
	}
}

void PacketLoader::SetPacketBuffer(int clientID, std::vector<BYTE>* buffer)
{
	packetBuffers[clientID] = buffer;
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

	//클라이언트가 보내는 패킷을 항상 cmdCnt와 commands뿐.
	//항상 data는 비운다.

	data.clear();

	SceneType type = SCENE_MGR->GetGameData().clientLocations[crntClientID];

	if (packetBuffer == nullptr || packetBuffer->empty()) {
		return notAlloc; // error
	}

	if (type == SceneType::Lobby || type == SceneType::Battle) {
		cmd = (BYTE)(packetBuffer->front()); // 1byte
		packetBuffer->erase(packetBuffer->begin());
	}
	else if (type == SceneType::Stage) {
		cmd = (BYTE)(*(packetBuffers[crntClientID]->begin() + 1)); // 1byte
		packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin() + 1);

		if (cmd == (BYTE)ClientStageCmd::EnterStage) {
			data.push_back(*(packetBuffers[crntClientID]->begin() + 1)); // ClientStageData
			packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin() + 1);
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
