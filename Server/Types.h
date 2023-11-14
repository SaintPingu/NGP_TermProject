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

	SOCKET_EXISTED,
	ERROR_CLOSE_SOCKET,
	CLIENT_CAN_NOT_ACCEPT_ANYMORE,
	CLIENT_DISCONNECT,

// SOCKET 관련 FAIL
	SERVER_SOCKET_CREATE_FAIL,
	SERVER_SOCKET_CONNECT_FAIL,
	SERVER_SOCKET_BIND_FAIL  ,
	SERVER_SOCKET_LISTEN_FAIL,
	ACCEPT_API_ERROR	,

// RECV 관련 FAIL 
	RECV_API_ERROR		,	
	RECV_REMOTE_CLOSE	,

// SEND 관련 FAIL
	SEND_REMOTE_CLOSE	,		
	SEND_SIZE_ZERO		,
	SEND_ERROR			,

// CLIENT 관련 FAIL
	CLIENT_NOT_CONNECTED,
	FORCING_CLOSE,
};