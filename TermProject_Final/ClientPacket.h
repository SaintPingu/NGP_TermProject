#pragma once
#include "../Server/PacketStruct.h"

class PacketGenerator
{
private:
	CommandList* cmdList = NULL;

public:
	PacketGenerator(){}
	~PacketGenerator(){}

	Packet GeneratePacket();
	CommandList* GetCommandList() { return cmdList; }
};

class PacketLoader
{
public:
	PacketLoader(){}
	~PacketLoader(){}

	PacketBuffer* buffer             = NULL;
	bool PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList,SceneType scenetype);
	std::vector<BYTE> PopData();

	void SetPacketBuffer(PacketBuffer& packet)
	{
		buffer = &packet;
	}
};

