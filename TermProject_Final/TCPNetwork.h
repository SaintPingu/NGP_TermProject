#pragma once

// 2023-11-19-SUN (장재문)-클라이언트 네트워크 기능을 추가하기 위해 TCPNetwork 생성 

/* +-----------------------------
	TCPNetwork ←←← ListenNetwork
	   ↑
	PacketNetwork
	   ↑
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
	TResult CreateSocket(); // 윈도우 소켓 초기화 및 생성 
	TResult CloseSocket();  // 윈도우 소켓 해제 

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
	TCPNetwork(const TCPNetwork& Origin) = delete; // 복사 생성 불가  
	~TCPNetwork();

};

