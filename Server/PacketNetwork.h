#pragma once
#include "TCPNetwork.h"
#include "ServerPacket.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.
// 2023-11-10-FRI (장재문) : PacketBuffer 생성 및 기본 함수 생성

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
	TResult SendTerminatePacket();

public:
	PacketBuffer& GetPacketBuffer() { return PacketBuf; }
	void SetPacketBuffer(const Packet& packet);


public:
	PacketNetwork();
	~PacketNetwork();

};

