#pragma once
#include "../Server/PacketStruct.h"

enum class ClientLobbyCmd { Terminate = 0, MoveLeft, MoveRight, MoveUp, MoveDown };

struct ClientLobbyPacket {
	BYTE dataLen;
	BYTE cmdCnt;
	ClientLobbyCmd* cmds;
};

class PacketBuffer : LobbyPacket, StagePacket, BattlePacket {};
// PacketBuffer 구현시 제거

class PacketGenerator
{
	CommandList* cmdList;
	ClientLobbyPacket GenLobbyPacket();
};

class PacketLoader
{
	LobbyPacket* LobbyPacketbuffer   = NULL;
	StagePacket* StagePacketbuffer   = NULL;
	BattlePacket* BattlePacketbuffer = NULL;

	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList);
	std::vector<BYTE> PopData();
};

