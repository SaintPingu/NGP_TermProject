#pragma once

#include "stdafx.h"
#include "../Common.h"
#include <bitset>


// 2023-11-06 (���繮) : ��Ŷ ���� ������ ���� / bitset�� �̿��� ��Ʈ���� �ɰ����� ������ �����Դϴ�.. + ��ȹ���� PlayerCount[n] ���� �Ǿ��ִµ� ���⼭ �� [n]����..? + ���� �����Ϳ����� vector ������ - ���� �ű� ��������..
// 2023-11-08 (���繮) : ��Ŷ ���� ������ ���� / ����Ʈ ���� - ���� ���� ������Ʈ ��..




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
		PlayerLobbyData*			 PlayersData;   // [Player1-LobbyData][Player2-LobbyData] ���� ����  
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
		EnemyData*				Enemys;     // [EnemyData][EnemyData][EnemyData]...[EnemyData] - EnemyCnt ���� ��ŭ ���� �迭 
	};
	/// +----------------------
	///	   BOSS SKILL - BATTLE
	/// ----------------------+	
	struct BossSkillBattleData
	{
		BYTE					EffectCnt;  // 1  BYTE
//		Effect*					Effects;	// [Effect][Effect][Effect]...[Effect] - EffectCnt ���� ��ŭ ���� �迭 
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
		BulletBattleData*	    BulletsData;	// [BulletBattleData][BulletBattleData]...[BulletBattleData] - BulletCnt ���� ��ŭ ���� �迭 
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