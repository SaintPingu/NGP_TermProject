#pragma once
#include "TCPNetwork.h"

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.

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

