#pragma once
#include "TCPNetwork.h"
#include "ClientPacket.h"

// 2023-11-19-SUN (장재문) : 클라이언트 전용 패킷 네트워크 생성 

/// +--------------
///	 PacketNetwork
/// --------------+	
class PacketNetwork : public TCPNetwork
{
private:
	PacketBuffer PacketBuf{};


public:
	virtual TResult Init() override;

public:
	virtual TResult SendPacket();
	virtual TResult RecvPacket();

public:
	PacketBuffer& GetPacketBuffer() { return PacketBuf; }
	void SetPacketBuffer(const Packet& packet);


public:
	PacketNetwork();
	~PacketNetwork();

};

