#pragma once
#include "stdafx.h"


enum class ClientLobbyCmd { Terminate = 0, MoveLeft, MoveRight, MoveUp, MoveDown };

struct ClientLobbyPacket {
	BYTE dataLen;
	BYTE cmdCnt;
	ClientLobbyCmd* cmds;
};

class PacketBuffer {};
// PacketBuffer 구현시 제거

class PacketGenerator
{
	CommandList* cmdList;
	ClientLobbyPacket GenLobbyPacket();
	
};

class PacketLoader
{
	PacketBuffer* buffer;
	bool PopCommand(BYTE& cmd, std::vector<BYTE*>&);
	std::vector<BYTE*> PopData();
};

