#pragma once

// 2023-11-19-SUN (���繮)-Ŭ���̾�Ʈ ��Ʈ��ũ ����� �߰��ϱ� ���� TCPNetwork ���� 

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

public:
	/// +------------------
	///    SOCKET OPTION
	/// ------------------+ 

	bool SetLinger(uint16 onoff, uint16 linger);
	bool SetReuseAddress(bool flag);
	bool SetRecvBufferSize(int32 size);
	bool SetSendBufferSize(int32 size);
	bool SetTcpNoDelay(bool flag);


	/// +--------------
	///	    G E T
	/// --------------+	
	const SOCKET& GetSocket();
	const SOCKADDR_IN& GetSocketAddr();


public:
	TCPNetwork();
	TCPNetwork(const TCPNetwork& Origin) = delete; // ���� ���� �Ұ�  
	~TCPNetwork();

};

