#pragma once

#include "stdafx.h"
#include "../Common.h"
#include <bitset>


// 2023-11-06 (장재문) : 패킷 내부 데이터 생성 / bitset을 이용해 비트별로 쪼개가며 구현할 예정입니다.. + 기획서에 PlayerCount[n] 으로 되어있는데 여기서 왜 [n]이지..? + 최종 데이터에서는 vector 빼야함 - 보낼 거기 때문이지..
// 2023-11-08 (장재문) : 패킷 내부 데이터 정리 / 바이트 정리 - 세부 사항 업데이트 중..




/* +--------------------------------------------------------------------------
	LobbyData
	{
		* PlayerLobbyData
			[ 1 ][ 9 ][ 9 ]...[ 9 ]					( 1 + 9 * n ) BYTE
	}
   ---------------------------------------------------------------------------+ */

/// +--------------
///	    LOBBY
/// --------------+	
namespace Lobby
{
	/// +--------------------
	///	    PLAYER - LOBBY
	/// --------------------+	
	struct PlayerLobbyData // 9 BYTE
	{
		BYTE			Pid_Mov_Dir;	// 1 BYTE - ( 0b [7][6][5][4][3]PlayerID [2]IsMoving [1][0]Dir )
		Vector2			Pos;			// 4 BYTE + 4 BYTE
	};

	/// +--------------
	///	  LOBBY DATA
	/// --------------+	
	struct LobbyData // 1 + ( 9 * n BYTE )
	{
		BYTE						 PlayerCnt;		// 2
		PlayerLobbyData*			 PlayersData;   // [Player1-LobbyData][Player2-LobbyData] 동적 생성  
	};

};


/* +--------------------------------------------------------------------------
	BattleData
	{
		* PlayerBattleData
			[ 1 ][ 8 ]								( 1 + 8 ) BYTE

		* EnemyBattleData
			[ 1 ][ 9 ][ 9 ][ 9 ]...[ 9 ]			( 1 + 9 * n ) BYTE

		* BulletsBattleData
			[ 1 ][ 17 ][ 17 ][ 17 ]...[ 17 ]		( 1 + 17 * n ) BYTE

		* BossSkillBattleData
			[ 1 ][ 9 ][ 9 ][ 9 ]...[ 9 ]			( 1 + 9 * n ) BYTE
	}
   ---------------------------------------------------------------------------+ */


/// +--------------
///	    BATTLE
/// --------------+	
namespace Battle
{
	/// +--------------------
	///	    PALYER - BATTLE
	/// --------------------+	
	struct PlayerBattleData   // 9 BYTE
	{
		BYTE		PlayerID;
		Vector2		Pos;
	};

	/// +--------------
	///	  ENEMY DATA
	/// --------------+	
	struct EnemyData          // 9 BYTE
	{
		
		BYTE		TypeDirActPad;    // 1 BYTE - ( 0b [7][6] Type [5][4] Dir [3][2] Action [1][0] Padding)
		Vector2		Pos;
	};

	struct EnemyBattleData
	{
		BYTE					EnemyCnt;	// 1  BYTE
		EnemyData*				Enemys;     // [EnemyData][EnemyData][EnemyData]...[EnemyData] - EnemyCnt 개수 만큼 동적 배열 
	};
	/// +----------------------
	///	   BOSS SKILL - BATTLE
	/// ----------------------+	
	struct BossSkillBattleData
	{
		BYTE					EffectCnt;  // 1  BYTE
//		Effect*					Effects;	// [Effect][Effect][Effect]...[Effect] - EffectCnt 개수 만큼 동적 배열 
	};
	
	/// +--------------------
	///	    BULLET - BATTLE
	/// --------------------+	
	struct BulletBattleData	  // 17 BYTE
	{
		BYTE		bulletType;
		Vector2		Pos;
		Vector2		Dir;
	};
	/// +--------------
	///	  BULLET DATA
	/// --------------+	
	struct BulletsBattleData
	{
		BYTE				    BulletCnt;		// 1  BYTE
		BulletBattleData*	    BulletsData;	// [BulletBattleData][BulletBattleData]...[BulletBattleData] - BulletCnt 개수 만큼 동적 배열 
	};


	/// +--------------
	///	  BATTLE DATA
	/// --------------+	
	const UINT MaxBattlePlayerCnt = 2;
	struct BattleData
	{
		PlayerBattleData		PlayerBattleData[MaxBattlePlayerCnt];
		EnemyBattleData			EnemyData;
		BulletsBattleData		BulletData;
		BossSkillBattleData		BossEffectData;
	};
};