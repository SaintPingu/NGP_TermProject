#include "stdafx.h"
#include "ClientPacket.h"


bool isGenPacket = false;

Packet PacketGenerator::GeneratePacket()
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList를 비운다.

	Packet packet;
	isGenPacket = true;

	if (pCommandList.size() == 0) {
		packet.push_back(0);
		return packet;
	}
	else {
		BYTE len = pCommandList.size() + sizeof(BYTE);
		packet.push_back(len); // Datalen
		packet.push_back(len - 1); // cntcmd
	}

	for (int i = 0;i < pCommandList.size();++i) {
		packet.push_back(pCommandList[i]); //data
	}

	return packet;
}

bool PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList)
{ //dataLen은 버퍼에 들어오지 않음.
	//항상 cmdList는 비운다.
	cmdList.clear();

	//if (로비)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerLobbyCmd::GoStage) {
			cmdList.push_back(*buffer->begin()); // 1byte StageElement
			buffer->erase(buffer->begin()); 	
		}
		return true;
	}
	//else if (스테이지)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerStageCmd::GoBattle) { // ClientStageData other
			for (int i = 0; i < buffer->size();++i) {
				cmdList.push_back((*buffer)[i]);
			}
		}
		buffer->clear(); // stage는 data가 따로 없으므로 여기서 Clear

		return true;
	}
	//else if (배틀)
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
