#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.

/// +--------------
///	 TCPListenNetwork
/// --------------+	

class TCPListenNetwork : public TCPNetwork
{
private:
	std::vector<SOCKET*>	NewClientSocks; // ���� ������ Ŭ���̾�Ʈ ���� ( ����ȭ ���� )  -->  ClientMgr 
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

