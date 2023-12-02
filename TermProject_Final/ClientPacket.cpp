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

bool PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList, SceneType scenetype)
{
	if (buffer->empty()) {
		return false;
	}

	//dataLen은 버퍼에 들어오지 않음.
	//항상 cmdList는 비운다.
	cmdList.clear();

	if (scenetype == SceneType::Lobby)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerLobbyCmd::GoStage) {
			cmdList.push_back(*buffer->begin()); // 1byte StageElement
			buffer->erase(buffer->begin()); 	
		}
		return false;
	}
	else if (scenetype == SceneType::Stage)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerStageCmd::GoBattle) { // ClientStageData other
			for (int i = 0; i < buffer->size();++i) {
				cmdList.push_back((*buffer)[i]);
			}
		}
		buffer->clear(); // stage는 data가 따로 없으므로 여기서 Clear

		return false;
	}
	else if (scenetype == SceneType::Battle)
	{ // 배틀에서는 cmdCnt 는 buffer로 들어오기때문에 PopCommand 이전에 1바이트를 지우고 시작하자.
		cmd = (BYTE)(*buffer->begin());
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerBattleCmd::Hit || cmd == (BYTE)ServerBattleCmd::UpdateMP) {
			// float hp || float mp
			cmdList.insert(cmdList.begin(), buffer->begin(), buffer->begin() + sizeof(float));
			buffer->erase(buffer->begin(), buffer->begin() + sizeof(float));
		}
		else if (cmd == (BYTE)ServerBattleCmd::CreateEffect) {
			// char type , Vector2 pos ==> float[2]
			cmdList.push_back((BYTE)(*buffer->begin()));
			buffer->erase(buffer->begin());

			//cmdList.begin() => type
			cmdList.insert(cmdList.begin() + 1, buffer->begin(), buffer->begin() + (sizeof(float) * 2));
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
