#include "stdafx.h"
#include "TCPListenNetwork.h"
// 2023-11-08-WED (장재문) : 리슨 네트워크 로직 생성  


TCPListenNetwork::TCPListenNetwork()
{
}

TCPListenNetwork::~TCPListenNetwork()
{
}

TResult TCPListenNetwork::Logic()
{
    while (true)
    {
        std::cout << "\t-> Hello Listen Network\n";
        // TODO : 클라이언트 접속 - ACCEPT -> ClientMgr 관리 ( 동기화 주의 )  

        break;
    }
    return TResult();

}

TResult TCPListenNetwork::BindListen(short PortNum)
{
    return TResult();
}

TResult TCPListenNetwork::Accept()
{
    return TResult();
}

void TCPListenNetwork::InsertSocket(SOCKET& socket)
{
}

