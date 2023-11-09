#pragma once
#include <unordered_map>
#include "../Server/PacketStruct.h"

enum DataType : BYTE { Lobby = 0, Stage, Battle };

class PacketGenerator
{
	Lobby::LobbyData lobbyData;
	//StageData stageData; /Stage�� cmd�� ����?
	Battle::BattleData battleData;
	
	void EncodeData();
	Packet EncodePacket(CommandList* cmdList, DataType type);
};

class PacketLoader
{
	std::unordered_map<int, PacketBuffer*> packetBuffers;
	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList);
	std::vector<BYTE> PopData();
};

