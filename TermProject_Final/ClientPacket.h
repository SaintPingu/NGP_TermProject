#pragma once

class PacketGenerator {
public:
	CommandList cmdList;

	PacketGenerator(){}
	~PacketGenerator(){}

	Packet GeneratePacket();
};

class PacketLoader {
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

