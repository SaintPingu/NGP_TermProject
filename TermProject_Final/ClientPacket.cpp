#include "stdafx.h"
#include "ClientPacket.h"

ClientLobbyPacket PacketGenerator::GenLobbyPacket()
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList를 비운다.

	ClientLobbyPacket packet;
	packet.dataLen = (BYTE)pCommandList.size() * sizeof(BYTE);

	if (packet.dataLen == 0) { // 아무 입력이 없다.
		packet.cmdCnt = 0;
		packet.cmds = NULL;
		return packet;
	}

	packet.cmdCnt = (BYTE)pCommandList.size();
	packet.cmds = new ClientLobbyCmd[packet.cmdCnt];
	for (int i = 0; i < packet.cmdCnt;++i) {
		packet.cmds[i] = (ClientLobbyCmd)(pCommandList[i]);
	}

	return packet;
}

bool PacketLoader::PopCommand(BYTE& cmd, std::vector<BYTE>& cmdList)
{
	//if (로비)
	{ // 로비일경우 커맨드는 하나
		static int cnt = 0;
		if (cnt == 0) { //dataLen remove
			buffer->erase(buffer->begin());
			++cnt;
			return true;
		}

		cmd = (BYTE)(*buffer->begin()); // 1byte
		buffer->erase(buffer->begin());
		if ((int)cmd == (int)ServerLobbyCmd::GoMenu) {
			cmdList.push_back((BYTE)(*buffer->begin())); // 1byte StageElement
			buffer->erase(buffer->begin(), buffer->begin() + 3); // [0],[1],[2] erase
			cnt = 0;
			return true;
		}
	}

	return false;
}

std::vector<BYTE> PacketLoader::PopData()
{
	std::vector<BYTE> pData;
	for (auto data = buffer->begin();data != buffer->end(); ++data) {
		pData.push_back(*data);
	}
	return pData;
}
