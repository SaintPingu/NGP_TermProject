#pragma once

#include "stdafx.h"
#include "../Common.h"
#include <bitset>


// 2023-11-06 (���繮) : ��Ŷ ���� ������ ���� / bitset�� �̿��� ��Ʈ���� �ɰ����� ������ �����Դϴ�.. + ��ȹ���� PlayerCount[n] ���� �Ǿ��ִµ� ���⼭ �� [n]����..? 
		 
/// +--------------
///	    LOBBY
/// --------------+	
namespace Lobby
{
	/// +--------------
	///	    PLAYER - LOBBY
	/// --------------+	
	struct PlayerLobbyData
	{
		// 1 BYTE
		std::bitset<5>	PlayerID;
		std::bitset<1>	IsMoving;
		std::bitset<2>	Dir;

		Vector2			Pos;
	};

	struct LobbyData
	{
		BYTE						 PlayerCount;
		std::vector<PlayerLobbyData> PlayersData;
	};

};

/// +--------------
///	    BATTLE
/// --------------+	
namespace Battle
{
	/// +--------------
	///	    PALYER - BATTLE
	/// --------------+	
	struct PlayerBattleData
	{
		BYTE		PlayerID;
		Vector2		Pos;
	};
	/// +--------------
	///	    ENEMY - BATTLE
	/// --------------+	
	struct EnemyData
	{
		// 1 BYTE
		std::bitset<2> Type;
		std::bitset<3> Dir;
		std::bitset<1> Action;
		std::bitset<3> Padding; 

		Vector2		   Pos;
	};

	struct EnemyBattleData
	{
		BYTE					EnemyCnt;	// [n]
		std::vector<EnemyData>	Enemys;		// Data
	};
	/// +--------------
	///	   BOSS SKILL - BATTLE
	/// --------------+	
	struct BossSkillBattleData
	{
		BYTE					EffectCnt; // [n]
//		std::vector<Effect>		Effects;
	};
	
	/// +--------------
	///	    BULLET - BATTLE
	/// --------------+	
	struct BulletBattleData
	{
		BYTE		bulletType;
		Vector2		Pos;
		Vector2		Dir;
	};
	struct BulletsBattleData
	{
		BYTE							BulletCnt;
		std::vector<BulletBattleData>	BulletsData;
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