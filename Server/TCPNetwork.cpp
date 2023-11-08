
#include "stdafx.h"
#include "TCPNetwork.h"

bool TCPNetwork::Init()
{
	return true;
}

TResult TCPNetwork::CreateSocket()
{
	return TResult();
}

TResult TCPNetwork::CloseSocket()
{
	return TResult();
}

const SOCKET& TCPNetwork::GetSocket()
{
	return TCP_Socket;
}

const SOCKADDR_IN& TCPNetwork::GetSocketAddr()
{
	return TCP_SockAddr;
}

TCPNetwork::TCPNetwork()
{

}

TCPNetwork::~TCPNetwork()
{

}
