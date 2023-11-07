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
	if (LobbyPacketbuffer) {
		//cmd = LobbyPacketbuffer->Command; // 서버에서 커맨드는 하나씩만 옴

		//GoMenu 명령이 들어 왔다면? char 1바이트 추가

		return true;
	}
	else if (StagePacketbuffer) {

		return true;
	}
	else if (BattlePacketbuffer) {

		return true;
	}

	return false;
}

std::vector<BYTE> PacketLoader::PopData()
{
	if (LobbyPacketbuffer) {
		//LobbyPacketbuffer->LobbyData
	}
	else if (StagePacketbuffer) {
	}
	else if (BattlePacketbuffer) {

	}
	return std::vector<BYTE>();
}
