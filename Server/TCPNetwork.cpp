
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
	int retval = ::closesocket(TCP_Socket);
	if (retval == SOCKET_ERROR)
		return TResult::ERROR_CLOSE_SOCKET;

	return TResult::SUCCESS;
}
