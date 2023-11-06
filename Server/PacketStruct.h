#pragma once
// 2023-11-05 (장재문) : 패킷 구조체들을 선언하기 위해서 생성되었습니다.
// 2023-11-06 (장재문) : 패킷 내부 데이터를 채웠습니다. Cmd enum 은 향후 추가할 예정입니다. 
//						+ 데이터 정보는 Data.h 에 있습니다. 


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