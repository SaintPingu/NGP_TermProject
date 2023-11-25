#include "stdafx.h"
#include "ServerNetwork.h"

TResult ServerNetwork::Init()
{
    return TResult();
}

void ServerNetwork::SetSocket(SOCKET& sock)
{
    TCP_Socket = sock;

	int addrlen = sizeof(TCP_SockAddr);
	getpeername(TCP_Socket, (struct sockaddr*)&TCP_SockAddr, &addrlen);
}

ServerNetwork::ServerNetwork()
{
}

ServerNetwork::~ServerNetwork()
{
}
