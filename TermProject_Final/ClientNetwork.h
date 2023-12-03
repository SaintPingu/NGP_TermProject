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
	bool			isTerminated{};
	bool			IsReceived{};

	ConnectFlag		curConnectFlag{}; // revc || send  
	bool			executeClientNet{};

	PacketGenerator packetGenerator;

	// 서버로부터 데이터를 전송할 준비가 완료되었다는 이벤트
	HANDLE sendPacket;
	virtual TResult SendPacket();
	virtual TResult RecvPacket();
	TResult RecvClientID(int& id);

	// 서버와 연결되었는가?
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