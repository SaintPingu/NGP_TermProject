#pragma once
#include "PacketNetwork.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.
// 2023-11-08-WED (장재문) : 

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

