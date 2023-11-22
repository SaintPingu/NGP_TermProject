#include "stdafx.h"
#include "ClientNetwork.h"
#include "TCPNetwork.h"
#include "PacketNetwork.h"

extern HANDLE recvPacket;

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
		// recv 구동 
		curConnectFlag = ConnectFlag::recv;

		// send 구동 
		curConnectFlag = ConnectFlag::send;

	}
}

// 23-11-21 최정일 SendPacket에서 패킷을 만들고 Send
TResult ClientNetwork::SendPacket()
{
	Packet packet = packetGenerator.GeneratePacket();

	SetPacketBuffer(packet);

	PacketNetwork::SendPacket();
	return TResult();
}

TResult ClientNetwork::RecvPacket()
{
	PacketNetwork::RecvPacket();

	SetEvent(recvPacket);

	return TResult();
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

