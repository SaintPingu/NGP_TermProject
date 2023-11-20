#include "stdafx.h"
#include "ClientNetwork.h"
#include "TCPNetwork.h"
#include "PacketNetwork.h"

ClientNetwork::ClientNetwork()
{
}

ClientNetwork::~ClientNetwork()
{
}

TResult ClientNetwork::Init()
{


    return TResult();
}

void ClientNetwork::Logic()
{
	while (executeClientNet)
	{
		// recv ���� 
		curConnectFlag = ConnectFlag::recv;

		// send ���� 
		curConnectFlag = ConnectFlag::send;

	}
}



TResult ClientNetwork::Init(std::string serverIP, short portnum)
{
	// 1. Socket
	if (CreateSocket() == TResult::SERVER_SOCKET_CREATE_FAIL)
		return TResult::FAIL;

	// 2. Connect 
	int				retval{};
	SOCKADDR_IN		serveraddr{};
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIP.c_str(), &serveraddr.sin_addr);

	serveraddr.sin_port = htons(portnum);
	retval = connect(GetSocket(), (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return TResult::FAIL;


    return TResult::SUCCESS;
}

