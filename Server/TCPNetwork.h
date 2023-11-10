#pragma once

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-10-SUN (���繮) : ���� ���� ���� CreateSocket(), CloseSocket()

/* +-----------------------------
	TCPNetwork ���� ListenNetwork
	   ��
	PacketNetwork
	   ��
	ServerNetwork
    -----------------------------+ */

/// +--------------
///	  TCPNetwork
/// --------------+	

class TCPNetwork
{

protected:
	SOCKET		TCP_Socket;
	SOCKADDR_IN TCP_SockAddr;


public:
	virtual TResult Init() = 0;

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

