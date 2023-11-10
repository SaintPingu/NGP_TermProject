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
	TResult SendPacket();
	TResult RecvPacket();

public:
	PacketBuffer* GetPacketBuffer();


public:
	PacketNetwork();
	~PacketNetwork();

};

