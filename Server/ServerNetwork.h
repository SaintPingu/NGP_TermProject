#pragma once
#include "PacketNetwork.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-08-WED (���繮) : 

/// +--------------
///	 ServerNetwork
/// --------------+	
class ServerNetwork : public PacketNetwork
{

private:
	bool	IsTerminated{};
	bool    IsReceived{};

public:
	virtual TResult Init() override;
	void SetSocket(SOCKET& sock);

	TResult SendClientID(int id);


public:
	bool IsTerminate() { return IsTerminated; }

public:
	ServerNetwork();
	~ServerNetwork();
};

