#pragma once
#include "PacketNetwork.h"

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.

/// +--------------
///	 ServerNetwork
/// --------------+	
class ServerNetwork : public PacketNetwork
{
private:

public:
	ServerNetwork();
	~ServerNetwork();
};

