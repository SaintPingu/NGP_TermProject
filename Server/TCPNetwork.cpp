
#include "stdafx.h"
#include "TCPNetwork.h"


TCPNetwork::TCPNetwork()
{

}

TCPNetwork::~TCPNetwork()
{

}


const SOCKET& TCPNetwork::GetSocket()
{
	return TCP_Socket;
}

const SOCKADDR_IN& TCPNetwork::GetSocketAddr()
{
	return TCP_SockAddr;
}


TResult TCPNetwork::CreateSocket()
{
	/// +--------------
	///	  1. Socket
	/// --------------+	
	TCP_Socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (TCP_Socket == INVALID_SOCKET)
		return TResult::SERVER_SOCKET_CREATE_FAIL;
	
	return TResult::SUCCESS;

}

TResult TCPNetwork::CloseSocket()
{
	/// +--------------
	///	     Close
	/// --------------+	
	if (TCP_Socket != NULL)
	{
		// 소켓을 닫습니다.
		int retval = ::closesocket(TCP_Socket);
		if (retval == SOCKET_ERROR)
			return TResult::ERROR_CLOSE_SOCKET;
		TCP_Socket   = NULL;
		TCP_SockAddr = SOCKADDR_IN();

	}
	else
	{
		// 소켓이 이미 닫혀있습니다. 
		return TResult::ERROR_CLOSE_SOCKET;
	}

	return TResult::SUCCESS;
}

bool TCPNetwork::SetLinger(uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SOCKET_ERROR != ::setsockopt(TCP_Socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&option), sizeof(LINGER));
}

bool TCPNetwork::SetReuseAddress(bool flag)
{
	return SOCKET_ERROR != ::setsockopt(TCP_Socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag), sizeof(bool));
}

bool TCPNetwork::SetRecvBufferSize(int32 size)
{
	return SOCKET_ERROR != ::setsockopt(TCP_Socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&size), sizeof(int32));
}

bool TCPNetwork::SetSendBufferSize(int32 size)
{
	return SOCKET_ERROR != ::setsockopt(TCP_Socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&size), sizeof(int32));
}

bool TCPNetwork::SetTcpNoDelay(bool flag)
{
	return SOCKET_ERROR != ::setsockopt(TCP_Socket, SOL_SOCKET, TCP_NODELAY, reinterpret_cast<char*>(&flag), sizeof(bool));
}
