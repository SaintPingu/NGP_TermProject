#pragma once
// 2023-11-05 (���繮) : ��Ŷ ����ü���� �����ϱ� ���ؼ� �����Ǿ����ϴ�.
// 2023-11-06 (���繮) : ��Ŷ ���� �����͸� ä�����ϴ�. Cmd enum �� ���� �߰��� �����Դϴ�. 
//						+ ������ ������ Data.h �� �ֽ��ϴ�. 


#include "Data.h"

/// +--------------
///	    LOBBY
/// --------------+	
struct LobbyPacket
{
	BYTE				DataLen;
	// ServerLobbyCmd	Command;
	Lobby::LobbyData	LobbyData;
};


/// +--------------
///	    STAGE
/// --------------+	
struct StagePacket
{
	BYTE				DataLen;
	// ServerStageCmd	command;
};


/// +--------------
///	    BATTLE
/// --------------+	
struct BattlePacket
{
	int32				DataLen;
	BYTE				CmdCnt; // [n]
//	ServerBattleCmd		Command;
	Battle::BattleData	BattleData;
};