#pragma once
#include "PacketNetwork.h"

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

public:
	virtual TResult Init() override;
	TResult Init(std::string serverIP, short portnum);

	void Logic();


public:
	bool IsTerminate() { return IsTerminated; }

public:
	ClientNetwork();
	~ClientNetwork();
};