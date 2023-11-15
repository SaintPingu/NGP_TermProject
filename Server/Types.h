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
	FAIL,
	SUCCESS,

	SOCKET_EXISTED,							// 소켓이 이미 존재함
	ERROR_CLOSE_SOCKET,						// 소켓 닫기 에러
	CLIENT_CAN_NOT_ACCEPT_ANYMORE,			// 클라이언트를 더이상 받을 수 없음
	CLIENT_DISCONNECT,						// 클라이언트 연결 끊어짐 
	ERROR_SYNC_ISSUE,						// 동기화 문제 오류

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