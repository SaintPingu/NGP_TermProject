#pragma once
#include "PacketNetwork.h"
#include "ClientPacket.h"

// 2023-11-19-SUN (장재문) : 클라이언트 네트워크 생성 

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
public:
	virtual TResult Init() override;
	TResult Init(std::string serverIP, short portnum);

	void Logic();

	virtual TResult SendPacket();
	virtual TResult RecvPacket();

	PacketGenerator GetPacketGenerator() { return packetGenerator; }
public:
	bool IsTerminate() { return IsTerminated; }

public:
	ClientNetwork();
	~ClientNetwork();
};