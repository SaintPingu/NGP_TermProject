#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.

/// +--------------
///	 TCPListenNetwork
/// --------------+	

class TCPListenNetwork : public TCPNetwork
{
private:
	Mutex TCPListen_Mutex;

public:
	TResult BindListen(short PortNum);
	TResult Accept();
	void	InsertSocket(SOCKET& socket);

public:

	TCPListenNetwork();
	~TCPListenNetwork();

};

