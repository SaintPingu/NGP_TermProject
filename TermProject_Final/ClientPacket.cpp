#include "stdafx.h"
#include "ClientPacket.h"


bool isGenPacket = false;

Packet PacketGenerator::GeneratePacket()
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList�� ����.

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
{ //dataLen�� ���ۿ� ������ ����.
	//�׻� cmdList�� ����.
	cmdList.clear();

	//if (�κ�)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerLobbyCmd::GoStage) {
			cmdList.push_back(*buffer->begin()); // 1byte StageElement
			buffer->erase(buffer->begin()); 	
		}
		return true;
	}
	//else if (��������)
	{
		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());

		if (cmd == (BYTE)ServerStageCmd::GoBattle) { // ClientStageData other
			for (int i = 0; i < buffer->size();++i) {
				cmdList.push_back((*buffer)[i]);
			}
		}
		buffer->clear(); // stage�� data�� ���� �����Ƿ� ���⼭ Clear

		return true;
	}
	//else if (��Ʋ)
	{ // ��Ʋ������ cmdCnt �� buffer�� �����⶧���� PopCommand ������ 1����Ʈ�� ����� ��������.
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
