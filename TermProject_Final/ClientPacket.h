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
	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList);
	std::vector<BYTE> PopData();
};

