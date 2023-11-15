#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-10-FRI (���繮) : �ʱ�ȭ



/// +--------------
///	 TCPListenNetwork
/// --------------+	

struct AcceptInfo
{
	int			ID;
	TResult		Result{};
	std::string IP;
	SOCKET		Socket;

	void SetSocket(SOCKET& sock) { Socket = sock; }
};

class TCPListenNetwork : public TCPNetwork
{
private:
	short					serverPort;
	bool					executeListen;
	
	std::vector<SOCKET*>	newClientSocks; // ���� ������ Ŭ���̾�Ʈ ���� ( ����ȭ ���� )  -->  ClientMgr 
	Mutex					mute;

	bool					active = true;

public:
	virtual TResult Init() override;

public:
	TResult		Logic();
	TResult		BindListen(short PortNum);
	AcceptInfo	Accept();
	void		InsertSocket(SOCKET& socket);
	void		Stop() { active = false; }
	void		Exit() { executeListen = false; TCPNetwork::CloseSocket(); }
	void		Execute() { executeListen = true; }


public:
	void SetPortNum(short port) { serverPort = port; }

public:

	TCPListenNetwork();
	~TCPListenNetwork();

};

