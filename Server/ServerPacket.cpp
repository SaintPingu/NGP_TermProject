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
			std::bitset<2> dir(static_cast<int>(players.at(clientID)->dir) - 1);
			std::bitset<8> byte(pid.to_string() + mov.to_string() + dir.to_string());

			playerlobbydata[i].Pid_Mov_Dir = static_cast<BYTE>(byte.to_ulong());
			playerlobbydata[i].Pos.x = players.at(clientID)->pos.x;
			playerlobbydata[i].Pos.x = players.at(clientID)->pos.y;
			++i;
		}

		lobbyData.PlayerCnt = players.size();
		lobbyData.PlayersData = playerlobbydata;
	}

	{// 배틀 생성 -- 일정 미뤄야할듯.

	}
}

Packet PacketGenerator::GeneratePacket(CommandList* cmdList, DataType type)
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList를 비운다.

	Packet packet;

	if (type == DataType::Lobby) {
		BYTE len = pCommandList.size() + sizeof(lobbyData);
		packet.push_back(len); // Datalen

		for (int i = 0;i < pCommandList.size();++i) {
			packet.push_back(pCommandList[i]); //ServerLobbyCmd
		}

		BYTE bytes[sizeof(lobbyData)];
		std::memcpy(bytes, &lobbyData, sizeof(lobbyData));

		for (int i = 0;i < sizeof(lobbyData);++i) {
			packet.push_back(bytes[i]); //ServerLobbyCmd
		}
	}
	else if (type == DataType::Stage) {
		BYTE len = pCommandList.size();
		packet.push_back(len); // Datalen

		for (int i = 0;i < pCommandList.size();++i) {
			packet.push_back(pCommandList[i]); //ServerStageCmd
		}

	}
	else if (type == DataType::Battle) {
		int len = pCommandList.size() + sizeof(battleData); // battle씬은 int 4바이트의 datalen을 보낸다.
		void* vlen = &len;
		for (int i = 0;i < sizeof(int);++i) {
			packet.push_back(((BYTE*)vlen)[i]); // Datalen	
		}

		packet.push_back(pCommandList.size()); // cmdCnt

		for (int i = 0;i < pCommandList.size();++i) {
			packet.push_back(pCommandList[i]); //ServerBattleCmd
		}

		BYTE bytes[sizeof(battleData)];
		std::memcpy(bytes, &battleData, sizeof(battleData));

		for (int i = 0;i < sizeof(battleData);++i) {
			packet.push_back(bytes[i]); //battleData
		}
	}

	return packet;
}

void PacketLoader::SetPacketBuffer(int clientID, std::vector<BYTE>* buffer)
{
	packetBuffers[clientID] = buffer;
}

int PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& data)
{
	if (packetBuffers.empty()) {
		return crntClientID;
	}

	if (crntClientID == -1) {
		crntClientID = packetBuffers.begin()->first;
	}
	//클라이언트가 보내는 패킷을 항상 cmdCnt와 data뿐.
	//항상 data는 비운다.

	data.clear();

	SceneType type = SCENE_MGR->GetGameData().clientLocations[crntClientID];

	if (type == SceneType::Town || type == SceneType::Battle) {
		cmd = (BYTE)(*(packetBuffers[crntClientID]->begin() + 1)); // 1byte
		packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin() + 1);
	}
	else if (type == SceneType::Stage) {
		cmd = (BYTE)(*(packetBuffers[crntClientID]->begin() + 1)); // 1byte
		packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin()+1);

		if (cmd == (BYTE)ClientStageCmd::EnterStage) {
			data.push_back(*(packetBuffers[crntClientID]->begin() + 1)); // ClientStageData
			packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin() + 1);
		}
	}

	return crntClientID;
}
