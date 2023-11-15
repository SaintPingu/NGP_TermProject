
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
