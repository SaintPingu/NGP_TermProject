#include "ClientPacket.h"


// Ŭ�� Lobby���� ����� ��Ŷ���� �ΰ� ������ ����.

ClientLobbyPacket PacketGenerator::GenLobbyPacket()
{//Lobby�� ������ ���� ��Ŷ
	std::vector<BYTE*> pCommandList = cmdList->GetCmdList(); //cmdList�� ���� ������.

	ClientLobbyPacket packet;
	packet.dataLen = pCommandList.size() * sizeof(BYTE);

	if (packet.dataLen == 0) { // �ƹ� �Է��� ����.
		packet.cmdCnt = 0;
		packet.cmds = NULL;
		return packet;
	}

	packet.cmdCnt = pCommandList.size();
	packet.cmds = new ClientLobbyCmd[packet.cmdCnt];
	for (int i = 0; i < packet.cmdCnt;++i) {
		packet.cmds[i] = (ClientLobbyCmd)(*pCommandList[i]);
	}


	return packet;
}