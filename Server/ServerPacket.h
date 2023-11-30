#pragma once
enum class DataType : BYTE { Lobby = 0, Stage, Battle };

class PacketGenerator
{

	void GenerateLobbyPacket(PacketBuffer& buffer, std::vector<BYTE>& cmdList);
	void GenerateStagePacket(PacketBuffer& buffer, std::vector<BYTE>& cmdList);
	int GetBattlePacketSize(size_t cmdListSize);
	void GenerateBattlePacket(PacketBuffer& buffer, std::vector<BYTE>& cmdList);

public:
	PacketGenerator() {}
	~PacketGenerator() {}

	Lobby::LobbyData lobbyData;
	//StageData stageData; /Stage는 cmd만 존재
	Battle::BattleData battleData;
	
	void GenerateData();
	void GeneratePacket(PacketBuffer& buffer, CommandList* cmdList, DataType type);

	void DeleteData();

};

class PacketLoader
{
public:
	PacketLoader() {}
	~PacketLoader() {}

	std::unordered_map<int, PacketBuffer*> packetBuffers;
	int crntClientID;
	int crntBufferIndex;

	void SetPacketBuffer(int clientID, std::vector<BYTE>* buffer);
	int PopCommand(BYTE& cmd, std::vector<BYTE>& data);
	void Clear() { packetBuffers.clear(); }
};

