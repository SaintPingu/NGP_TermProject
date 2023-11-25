#include "stdafx.h"
#include "ClientNetwork.h"
#include "TCPNetwork.h"
#include "PacketNetwork.h"
#include "framework.h"

ClientNetwork::ClientNetwork()
{
	sendPacket = CreateEvent(NULL, FALSE, FALSE, NULL);
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
	isConnected = true;
	executeClientNet = true;
	framework->WakeForPacket();
	while (executeClientNet)
	{
		std::cout << "���� ���\n";
		// recv ����
		if (RecvPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::recv;
		framework->WakeForPacket();

		std::cout << "���� �Ϸ� �� �۽� ���\n";
		// send ���
		ResetEvent(sendPacket);
		WaitForSingleObject(sendPacket, INFINITE);

		std::cout << "�۽� ��\n";

		// send ����
		if (SendPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::send;

		std::cout << "�۽� �Ϸ�\n";

	}

	// Terminate ��� ����
	// ...

	std::cout << "���� ���� ����\n";
	// ���� ����
	CloseHandle(sendPacket);
	WSACleanup();
	isConnected = false;
}

// 23-11-21 ������ SendPacket���� ��Ŷ�� ����� Send
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

