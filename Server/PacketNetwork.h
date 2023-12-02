#pragma once
#include "TCPNetwork.h"
#include "ServerPacket.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-10-FRI (���繮) : PacketBuffer ���� �� �⺻ �Լ� ����

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

