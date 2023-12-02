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

	int id;
	RecvClientID(id);
	framework->SetClientID(id);

	framework->CompleteServerConnect();
	while (executeClientNet)
	{
		//std::cout << "���� ���\n";
		// recv ����
		curConnectFlag = ConnectFlag::RecvStart;
		if (RecvPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::RecvFinish;
		framework->WakeForPacket();
		//std::cout << "���� �Ϸ� �� �۽� ���\n";
		// send ���
		WaitForSingleObject(sendPacket, INFINITE);
		ResetEvent(sendPacket);

		// 12/02 �ε��� : ���� �������� ���� Ŀ��Ʈ ���� �� ���� ���� ����
		if (!executeClientNet) {
			break;
		}
		//std::cout << "�۽� ��\n";

		// send ����
		curConnectFlag = ConnectFlag::SendStart;
		if (SendPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::SendFinish;

		//std::cout << "�۽� �Ϸ�\n";

	}

	// Terminate ��� ����
	// ...

	std::cout << "���� ���� ����\n";
	// ���� ����
	CloseSocket();
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

TResult ClientNetwork::RecvClientID(int& id)
{
	int retval{};
	retval = recv(TCP_Socket, (char*)&id, sizeof(int), MSG_WAITALL);
	std::cout << "Clinet ID : " << id << std::endl;
	if (retval == SOCKET_ERROR) {
		err_display("Recv Error");
		return TResult::FAIL;
	}

	return TResult::NONE;
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

