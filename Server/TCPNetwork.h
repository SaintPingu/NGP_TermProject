#pragma once

// 2023-11-05 : 큰 틀을 잡기 위해서 미리 생성되었습니다.

/* +--------------------------------------
	TCPNetwork -------→ ListenNetwork
	   ↓
	PacketNetwork
	   ↓
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
	TResult CreateSocket(); // 윈도우 소켓 초기화 및 생성 
	TResult CloseSocket();  // 윈도우 소켓 해제 


	/// +--------------
	///	    G E T
	/// --------------+	
	const SOCKET&		GetSocket();
	const SOCKADDR_IN&	GetSocketAddr();


public:
	TCPNetwork();
	TCPNetwork(const TCPNetwork& Origin) = delete; // 복사 생성 불가  
	~TCPNetwork();

};

