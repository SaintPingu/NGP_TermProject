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


public:
	virtual bool Init();


public:
	TCPNetwork();
	~TCPNetwork();

};

