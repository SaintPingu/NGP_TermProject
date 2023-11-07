#include "stdafx.h"
#include "ClientPacket.h"

ClientLobbyPacket PacketGenerator::GenLobbyPacket()
{
	std::vector<BYTE> pCommandList = cmdList->GetCmdList(); //cmdList�� ����.

	ClientLobbyPacket packet;
	packet.dataLen = (BYTE)pCommandList.size() * sizeof(BYTE);

	if (packet.dataLen == 0) { // �ƹ� �Է��� ����.
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
		//cmd = LobbyPacketbuffer->Command; // �������� Ŀ�ǵ�� �ϳ����� ��

		//GoMenu ����� ��� �Դٸ�? char 1����Ʈ �߰�

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
