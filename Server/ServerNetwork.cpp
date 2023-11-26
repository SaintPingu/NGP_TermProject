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


TResult ServerNetwork::SendClientID(int id)
{
    int retval = send(TCP_Socket, (const char*)&id, sizeof(int), 0);
    if (retval == SOCKET_ERROR) {
        err_display("Send Error");
        return TResult::FAIL;
    }

    return TResult::NONE;
}

ServerNetwork::ServerNetwork()
{
}

ServerNetwork::~ServerNetwork()
{
}
