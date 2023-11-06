#pragma once

#include "stdafx.h"
#include "../Common.h"
#include <bitset>


// 2023-11-06 (장재문) : 패킷 내부 데이터 생성 / bitset을 이용해 비트별로 쪼개가며 구현할 예정입니다.. + 기획서에 PlayerCount[n] 으로 되어있는데 여기서 왜 [n]이지..? 
		 
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