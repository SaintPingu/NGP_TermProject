#include "stdafx.h"
#include "ClientPacket.h"


bool isGenPacket = false;

Packet PacketGenerator::GeneratePacket()
{
	std::vector<BYTE> pCommandList = cmdList.GetCmdList(); //cmdList를 비운다.

	Packet packet;
	isGenPacket = true;

	for (int i = 0;i < pCommandList.size();++i) {
		packet.push_back(pCommandList[i]); // commandList
	}

	return packet;
}

bool PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& cmdData, SceneType scenetype)
{
	if (buffer->empty()) {
		return false;
	}

	//dataLen은 버퍼에 들어오지 않음.
	//항상 cmdList는 비운다.
	cmdData.clear();

	if (scenetype == SceneType::Lobby)
	{
		cmd = buffer->front(); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerLobbyCmd::GoStage) {
			cmdData.push_back(buffer->front()); // 1byte StageElement
			buffer->clear();
		}
		return false;
	}
	else if (scenetype == SceneType::Stage)
	{
		cmd = buffer->front(); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerStageCmd::GoBattle) { // ClientStageData other
			cmdData.insert(cmdData.begin(), buffer->begin(), buffer->begin() + buffer->size());
		}
		buffer->clear(); // stage는 data가 따로 없으므로 여기서 Clear

		return false;
	}
	else if (scenetype == SceneType::Battle)
	{
		cmd = buffer->front();
		buffer->erase(buffer->begin());

		switch ((ServerBattleCmd)cmd) {
		case ServerBattleCmd::Hit:
		case ServerBattleCmd::UpdateMP:
			cmdData.insert(cmdData.begin(), buffer->begin(), buffer->begin() + sizeof(float));
			RemoveData(*buffer, sizeof(float));
			break;
		case ServerBattleCmd::CreateEffect:
		{
			size_t dataSize = sizeof(Battle::EffectData);
			cmdData.resize(dataSize);
			memcpy(cmdData.data(), buffer->data(), dataSize);
			buffer->erase(buffer->begin(), buffer->begin() + dataSize);
		}
			break;
		case ServerBattleCmd::BossPos:
			cmdData.insert(cmdData.begin(), buffer->begin(), buffer->begin() + sizeof(Vector2));
			RemoveData(*buffer, sizeof(Vector2));
			break;
		default:
			break;
		}

		return true;
	}

	return false;
}

std::vector<BYTE> PacketLoader::PopData()
{
	std::vector<BYTE> pData;
	for (int i = 0;i < buffer->size();++i) {
		pData.push_back((*buffer)[i]);
	}
	buffer->clear();

	return pData;
}
