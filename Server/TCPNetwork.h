#pragma once

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.
// 2023-11-10-SUN (장재문) : 내부 구현 시작 CreateSocket(), CloseSocket()

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

