#pragma once

// 2023-11-05 : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.

/* +--------------------------------------
	TCPNetwork -------�� ListenNetwork
	   ��
	PacketNetwork
	   ��
	ServerNetwork
    -------------------------------------+ */

/// +--------------
///	  TCPNetwork
/// --------------+	

class TCPNetwork
{
private:
	SOCKET		TCP_Socket;
	SOCKADDR_IN TCP_SockAddr;


public:
	virtual bool Init();

public:
	TResult CreateSocket(); // ������ ���� �ʱ�ȭ �� ���� 
	TResult CloseSocket();  // ������ ���� ���� 


	/// +--------------
	///	    G E T
	/// --------------+	
	const SOCKET&		GetSocket();
	const SOCKADDR_IN&	GetSocketAddr();


public:
	TCPNetwork();
	TCPNetwork(const TCPNetwork& Origin) = delete; // ���� ���� �Ұ�  
	~TCPNetwork();

};

