#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.

/// +--------------
///	 TCPListenNetwork
/// --------------+	

class TCPListenNetwork : public TCPNetwork
{
private:
	std::vector<SOCKET*>	NewClientSocks; // 새로 접속한 클라이언트 관리 ( 동기화 문제 )  -->  ClientMgr 
	Mutex					Mute;

public:
	TResult Logic();


	TResult BindListen(short PortNum);
	TResult Accept();
	void	InsertSocket(SOCKET& socket);

public:

	TCPListenNetwork();
	~TCPListenNetwork();

};

