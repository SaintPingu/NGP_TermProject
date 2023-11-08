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

// SOCKET 包访 FAIL
	SERVER_SOCKET_CREATE_FAIL,
	SERVER_SOCKET_BIND_FAIL  ,
	SERVER_SOCKET_LISTEN_FAIL,
	ACCEPT_API_ERROR	,

// RECV 包访 FAIL 
	RECV_API_ERROR		,	
	RECV_REMOTE_CLOSE	,

// SEND 包访 FAIL
	SEND_REMOTE_CLOSE	,		
	SEND_SIZE_ZERO		,
	SEND_ERROR			,

// CLIENT 包访 FAIL
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




