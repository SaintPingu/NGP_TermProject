#pragma once
#include "../Server/PacketStruct.h"

enum class ClientLobbyCmd { Terminate = 0, MoveLeft, MoveRight, MoveUp, MoveDown };

struct ClientLobbyPacket {
	BYTE dataLen;
	BYTE cmdCnt;
	ClientLobbyCmd* cmds;
};


class PacketGenerator
{
	CommandList* cmdList;
	ClientLobbyPacket GenLobbyPacket();
};

class PacketLoader
{
	PacketBuffer* buffer             = NULL;
	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList);
	std::vector<BYTE> PopData();
};

