#include "stdafx.h"
#include "TCPListenNetwork.h"
// 2023-11-08-WED (���繮) : ���� ��Ʈ��ũ ���� ����  


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
        // TODO : Ŭ���̾�Ʈ ���� - ACCEPT -> ClientMgr ���� ( ����ȭ ���� )  

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

