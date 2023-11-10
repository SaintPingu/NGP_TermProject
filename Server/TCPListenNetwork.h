#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.
// 2023-11-10-FRI (장재문) : 초기화



/// +--------------
///	 TCPListenNetwork
/// --------------+	

struct AcceptInfo
{
	TResult		Result{};
	std::string IP;
	SOCKET		Socket;

	void SetSocket(SOCKET& sock) { Socket = sock; }
};

class TCPListenNetwork : public TCPNetwork
{
private:
	short					ServerPort;

	std::vector<SOCKET*>	NewClientSocks; // 새로 접속한 클라이언트 관리 ( 동기화 문제 )  -->  ClientMgr 
	Mutex					Mute;

public:
	virtual TResult Init() override;

public:
	TResult		Logic();
	TResult		BindListen(short PortNum);
	AcceptInfo	Accept();
	void		InsertSocket(SOCKET& socket);


public:
	void SetPortNum(short port) { ServerPort = port; }

public:

	TCPListenNetwork();
	~TCPListenNetwork();

};

