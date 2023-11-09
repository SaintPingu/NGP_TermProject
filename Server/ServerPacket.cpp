#include "ServerPacket.h"

//////////////////////////////////////////////////////////
/// ������ ����
#include <map>
struct GameData {
	bool isBattleStart;
	StageElement crntStageType;
	std::map<int, SceneType> clientLocatio;
};
class Scene {
public:
};

struct LobbyPlayer {
	Vector2 pos;
	Dir dir;
	bool isMoving;
};

class LobbyScene : Scene {
public:
	std::map<int, LobbyPlayer> players;
	RECT building[18];
};

class SceneManager {
public:
	GameData gameData;
	LobbyScene lobbyScene;
	//StageScene stageScene;
	//BattleScene	battleScene;
};
SceneManager sceneManager;
//////////////////////////////////////////////////////////

void PacketGenerator::GenerateData()
{ // GenerateData

	{// �κ� ����
		LobbyScene& lobby = sceneManager.lobbyScene;

		Lobby::PlayerLobbyData* playerlobbydata = new Lobby::PlayerLobbyData[lobby.players.size()];
		for (int i = 0;i < lobby.players.size();++i) {
			playerlobbydata[i].Pid_Mov_Dir = (i << 3) + (lobby.players[i].isMoving << 2) + BYTE(lobby.players[i].dir);
			playerlobbydata[i].Pos.x = lobby.players[i].pos.x;
			playerlobbydata[i].Pos.y = lobby.players[i].pos.y;
		}
		lobbyData.PlayerCnt = lobby.players.size();
		lobbyData.PlayersData = playerlobbydata;
	}

	{// ��Ʋ ���� -- ���� �̷���ҵ�.

	}
}

Packet PacketGenerator::GeneratePacket(CommandList* cmdList, DataType type)
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList�� ����.

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
		int len = pCommandList.size() + sizeof(battleData); // battle���� int 4����Ʈ�� datalen�� ������.
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
	// buffer[crntBufferIndex]�� ���븸 �ű�� ����.
	*packetBuffers[clientID] = std::move(buffer[crntBufferIndex]);
}

int PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList)
{
	//client ID�� ��� ����?
	//Ŭ���̾�Ʈ�� ������ ��Ŷ�� �׻� cmdCnt�� cmdlist��.
	
	//�׻� cmdList�� ����.
	cmdList.clear();

	//if (�κ� || ��Ʋ)
	{
		cmd = (BYTE)(*(packetBuffers[crntClientID]->begin() + 1)); // 1byte
		packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin());
		
		return true;
	}
	//else if (��������)
	{
		cmd = (BYTE)(*(packetBuffers[crntClientID]->begin() + 1)); // 1byte
		packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin()+1);

		if (cmd == (BYTE)ClientStageCmd::EnterStage) {
			cmdList.push_back(*(packetBuffers[crntClientID]->begin() + 1)); // ClientStageData
			packetBuffers[crntClientID]->erase(packetBuffers[crntClientID]->begin() + 1);
		}
		return true;
	}

	return false;
}
