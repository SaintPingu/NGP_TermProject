#pragma once

// 2023-11-05-SUN (���繮) : ��Ŷ ����ü���� �����ϱ� ���ؼ� �����Ǿ����ϴ�.
// 2023-11-06-MON (���繮) : ��Ŷ ���� �����͸� ä�����ϴ�. Cmd enum �� ���� �߰��� �����Դϴ�. + ������ ������ Data.h �� �ֽ��ϴ�. 
// 2023-11-08-WED (���繮) : CMD enum �߰� 

#include "Data.h"
using PacketBuffer = std::vector<BYTE>;
using Packet = std::vector<BYTE>;

/// +--------------
///	    LOBBY
/// --------------+	
struct LobbyPacket
{
	BYTE				DataLen;
	ServerLobbyCmd		Command;
	Lobby::LobbyData	LobbyData;
};


/// +--------------
///	    STAGE
/// --------------+	
struct StagePacket
{
	BYTE				DataLen;
	ServerStageCmd		Command;
};


/// +--------------
///	    BATTLE
/// --------------+	
struct BattlePacket
{
	int32				DataLen;
	BYTE				CmdCnt; 
	ServerBattleCmd*    Command;		// [ServerBattleCmd][ServerBattleCmd]...[ServerBattleCmd]
	Battle::BattleData	BattleData;
};