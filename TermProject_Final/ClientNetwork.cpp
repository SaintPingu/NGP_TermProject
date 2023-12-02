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
		//std::cout << "수신 대기\n";
		// recv 구동
		curConnectFlag = ConnectFlag::RecvStart;
		if (RecvPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::RecvFinish;
		framework->WakeForPacket();
		//std::cout << "수신 완료 및 송신 대기\n";
		// send 대기
		WaitForSingleObject(sendPacket, INFINITE);
		ResetEvent(sendPacket);

		// 12/02 민동현 : 수신 과정에서 종료 커맨트 수신 시 서버 연결 종료
		if (!executeClientNet) {
			break;
		}
		//std::cout << "송신 중\n";

		// send 구동
		curConnectFlag = ConnectFlag::SendStart;
		if (SendPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::SendFinish;

		//std::cout << "송신 완료\n";

	}

	// Terminate 명령 전달
	// ...

	std::cout << "서버 연결 종료\n";
	// 연결 종료
	CloseSocket();
	CloseHandle(sendPacket);
	WSACleanup();
	isConnected = false;
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

