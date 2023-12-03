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
	bool			isTerminated{};
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

	void DisConnect();
public:
	virtual TResult Init() override;
	TResult Init(std::string serverIP, short portnum);

	void Logic();
	void Send() { SetEvent(sendPacket); }

	PacketGenerator& GetPacketGenerator() { return packetGenerator; }

	void Terminate() { isTerminated = true; SetEvent(sendPacket); }
public:
	bool IsTerminate() { return isTerminated; }

	bool IsConnected() { return isConnected; }

	ConnectFlag GetConnectFlag() const { return curConnectFlag; }

public:
	ClientNetwork();
	~ClientNetwork();
};