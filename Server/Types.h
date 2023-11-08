#pragma once
#include <mutex>
#include <atomic>

template<typename T>
using Atomic = std::atomic<T>;
using Mutex  = std::mutex;


/// +------------------
///	 SERVER ERROR CODE 
/// ------------------+	
enum class TResult : short
{
	NONE = 0,
	SUCCESS,

// SOCKET ���� FAIL
	SERVER_SOCKET_CREATE_FAIL,
	SERVER_SOCKET_BIND_FAIL  ,
	SERVER_SOCKET_LISTEN_FAIL,
	ACCEPT_API_ERROR	,

// RECV ���� FAIL 
	RECV_API_ERROR		,	
	RECV_REMOTE_CLOSE	,

// SEND ���� FAIL
	SEND_REMOTE_CLOSE	,		
	SEND_SIZE_ZERO		,
	SEND_ERROR			,

// CLIENT ���� FAIL
	CLIENT_NOT_CONNECTED,
	FORCING_CLOSE,
};

/// +------------------
///	 SERVER LOBBY CMD
/// ------------------+	
enum class ServerLobbyCmd
{
	GoMenu,
	GoStage,
	Quit,
	None,

};
/// +------------------
///	 SERVER STAGE CMD
/// ------------------+	
enum class ServerStageCmd
{
	GoTown,
	GoBattle,

};
/// +------------------
///	 SERVER BATTLE CMD
/// ------------------+	
enum class ServerBattleCmd
{
	Loss,
	Win,
	AcceptSkillQ,
	Hit,
	UpdateMP,
	CreateEffect,

};




