#pragma once
#include "../Server/PacketStruct.h"

class PacketGenerator
{
public:
	PacketGenerator(){}
	~PacketGenerator(){}

	CommandList* cmdList;
	Packet GeneratePacket();
};

class PacketLoader
{
public:
	PacketLoader(){}
	~PacketLoader(){}

	PacketBuffer* buffer             = NULL;
	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList,SceneType scenetype);
	std::vector<BYTE> PopData();
};

