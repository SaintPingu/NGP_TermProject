#include "stdafx.h"
#include "ClientPacket.h"


bool isGenPacket = false;

Packet PacketGenerator::GeneratePacket()
{
	std::vector<BYTE> pCommandList = cmdList.GetCmdList(); //cmdList를 비운다.

	Packet packet;
	isGenPacket = true;

	uint8 len = pCommandList.size();
	PushData(packet, &len, sizeof(uint8));

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
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerLobbyCmd::GoStage) {
			cmdData.push_back(buffer->front()); // 1byte StageElement
			buffer->clear();
		}
		return false;
	}
	else if (scenetype == SceneType::Stage)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerStageCmd::GoBattle) { // ClientStageData other
			for (int i = 0; i < buffer->size(); ++i) {
				cmdData.push_back((*buffer)[i]);
			}
		}
		buffer->clear(); // stage는 data가 따로 없으므로 여기서 Clear

		return false;
	}
	else if (scenetype == SceneType::Battle)
	{
		cmd = (BYTE)(*buffer->begin());
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerBattleCmd::Hit || cmd == (BYTE)ServerBattleCmd::UpdateMP) {
			// float hp || float mp
			cmdData.insert(cmdData.begin(), buffer->begin(), buffer->begin() + sizeof(float));
			buffer->erase(buffer->begin(), buffer->begin() + sizeof(float));
		}
		else if (cmd == (BYTE)ServerBattleCmd::CreateEffect) {
			// char type , Vector2 pos ==> float[2]
			cmdData.push_back((BYTE)(*buffer->begin()));
			buffer->erase(buffer->begin());

			//cmdData.begin() => type
			cmdData.insert(cmdData.begin() + 1, buffer->begin(), buffer->begin() + (sizeof(float) * 2));
			buffer->erase(buffer->begin(), buffer->begin() + (sizeof(float) * 2));
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
