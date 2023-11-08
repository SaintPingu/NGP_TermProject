#pragma once

// 2023-11-05-SUN (장재문) : 패킷 구조체들을 선언하기 위해서 생성되었습니다.
// 2023-11-06-MON (장재문) : 패킷 내부 데이터를 채웠습니다. Cmd enum 은 향후 추가할 예정입니다. + 데이터 정보는 Data.h 에 있습니다. 
// 2023-11-08-WED (장재문) : CMD enum 추가 

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