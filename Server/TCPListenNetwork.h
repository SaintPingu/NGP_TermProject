#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-10-FRI (���繮) : �ʱ�ȭ



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

	std::vector<SOCKET*>	NewClientSocks; // ���� ������ Ŭ���̾�Ʈ ���� ( ����ȭ ���� )  -->  ClientMgr 
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

