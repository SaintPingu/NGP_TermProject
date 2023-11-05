#pragma once
#include <mutex>
#include <atomic>

using BYTE   = unsigned char;
using int8   = __int8;
using int16  = __int16;
using int32  = __int32;
using int64  = __int64;
using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;


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
