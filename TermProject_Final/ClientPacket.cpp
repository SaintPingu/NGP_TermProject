#include "ClientPacket.h"


// 클라가 Lobby에서 만드는 패킷에는 부가 정보가 없음.

ClientLobbyPacket PacketGenerator::GenLobbyPacket()
{//Lobby에 있을때 만들 패킷
	std::vector<BYTE*> pCommandList = cmdList->GetCmdList(); //cmdList를 비우고 가져옴.

	ClientLobbyPacket packet;
	packet.dataLen = pCommandList.size() * sizeof(BYTE);

	if (packet.dataLen == 0) { // 아무 입력이 없다.
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