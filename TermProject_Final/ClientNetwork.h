#pragma once
#include "PacketNetwork.h"
#include "ClientPacket.h"

// 2023-11-19-SUN (���繮) : Ŭ���̾�Ʈ ��Ʈ��ũ ���� 

/// +--------------
///	 ClientNetwork
/// --------------+	
class ClientNetwork: public PacketNetwork
{

private:
	bool			IsTerminated{};
	bool			IsReceived{};

	ConnectFlag		curConnectFlag{}; // revc || send  
	bool			executeClientNet{};

	PacketGenerator packetGenerator;

	// �����κ��� �����͸� ������ �غ� �Ϸ�Ǿ��ٴ� �̺�Ʈ
	HANDLE sendPacket;
	virtual TResult SendPacket();
	virtual TResult RecvPacket();
	TResult RecvClientID(int& id);

	// ������ ����Ǿ��°�?
	bool isConnected = false;
public:
	virtual TResult Init() override;
	TResult Init(std::string serverIP, short portnum);

	void Logic();
	void Send() { SetEvent(sendPacket); }

	PacketGenerator& GetPacketGenerator() { return packetGenerator; }
public:
	bool IsTerminate() { return IsTerminated; }

	bool IsConnected() { return isConnected; }

public:
	ClientNetwork();
	~ClientNetwork();
};