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

void ClientNetwork::DisConnect()
{
	isConnected = false;
	framework->DisconnectServer();
	framework->WakeForPacket();
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
		TResult result = RecvPacket();
		if (result == TResult::FAIL || result == TResult::CLIENT_DISCONNECT) {
			DisConnect();	// ���� ����
			break;
		}
		//std::cout << "���� �Ϸ� �� �۽� ���\n";
		curConnectFlag = ConnectFlag::RecvFinish; 
		framework->WakeForPacket();

		// send ���
		WaitForSingleObject(sendPacket, INFINITE);
		ResetEvent(sendPacket);

		// 12-03 �ε��� : Ŭ���̾�Ʈ framework ������Ʈ ���� ���� ���� ��û �� ���� ��Ŷ�� ������ �����Ѵ�.
		if (IsTerminate()) {
			SendTerminatePacket();
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

	return PacketNetwork::SendPacket();
}

TResult ClientNetwork::RecvPacket()
{
	return PacketNetwork::RecvPacket();
}

TResult ClientNetwork::RecvClientID(int& id)
{
	int retval{};
	retval = recv(TCP_Socket, (char*)&id, sizeof(int), MSG_WAITALL);
	std::cout << "Client ID : " << id << std::endl;
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

