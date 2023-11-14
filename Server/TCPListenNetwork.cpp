#include "stdafx.h"
#include "TCPListenNetwork.h"
#include "ClientMgr.h"



// 2023-11-08-WED (장재문) : 리슨 네트워크 로직 생성  
// 2023-11-10-FRI (장재문) : Bind Listen Accept   


TCPListenNetwork::TCPListenNetwork()
    : executeListen(true)
{
}

TCPListenNetwork::~TCPListenNetwork()
{
}

TResult TCPListenNetwork::Init()
{
	if (TCP_Socket)
        return TResult::SOCKET_EXISTED;


    CreateSocket();
    BindListen(9000); // 임시 포트번호  
    
    return TResult::SUCCESS;    
}

TResult TCPListenNetwork::Logic()
{
    std::cout << "\t-> Listen Network 구동 중...\n";
    std::cout << "\t\t-> Accept()...wait\n";

    serverPort = 0;
    while (executeListen)
    {
       
#define _EXECUTE_SERVER_
#ifdef  _EXECUTE_SERVER_
        /// +-------------
        ///	  4. Accept
        /// -------------+
        AcceptInfo acceptinfo = TCPListenNetwork::Accept();
      
        /// +-------------------------
        ///	  Thread For Each Client.
        /// -------------------------+	
        TResult Res = CLIENT_MGR->RegisterConnectedClient(acceptinfo.IP, acceptinfo.Socket);
        if (Res == TResult::SUCCESS)
            CLIENT_MGR->CreateClientThread(acceptinfo.Socket, 0);
#endif

    }
    std::cout << "\t-> Listen Network 종료...\n";
    return TResult();

}

TResult TCPListenNetwork::BindListen(short PortNum)
{
    serverPort = PortNum;

    /// +--------------
    ///	  2. Bind
    /// --------------+
    memset(&TCP_SockAddr, 0, sizeof(TCP_SockAddr));

    TCP_SockAddr.sin_family      = AF_INET;             // IPv4
    TCP_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // host to network long
    TCP_SockAddr.sin_port        = htons(PortNum);      // host to network short
    int retval                   = ::bind(TCP_Socket, (SOCKADDR*)&TCP_SockAddr, sizeof(TCP_SockAddr));
    if (retval == SOCKET_ERROR)
        return TResult::SERVER_SOCKET_BIND_FAIL;

    /// +--------------
    ///	  3. Listen
    /// --------------+
    int RetVal = ::listen(TCP_Socket, SOMAXCONN);
    if (RetVal == SOCKET_ERROR)
        return TResult::SERVER_SOCKET_LISTEN_FAIL;

    return TResult::SUCCESS;
}


AcceptInfo TCPListenNetwork::Accept()
{
    AcceptInfo  Result      = {};
    SOCKET      ClientSock  = {};
    SOCKADDR_IN ClientAddr  = {};
    int         AddrLen     = sizeof(ClientAddr);
    
    /// +--------------
    ///	  4. Accept
    /// --------------+
    ClientSock = ::accept(TCP_Socket, (SOCKADDR*)&ClientAddr, &AddrLen);
    if (ClientSock == INVALID_SOCKET)
    {
        Result.Result = TResult::SERVER_SOCKET_CONNECT_FAIL;
        Result.Socket = NULL;
        return Result;
    }

#define _DEBUG_SERVER
#ifdef  _DEBUG_SERVER
    char Addr[INET_ADDRSTRLEN]{};
    inet_ntop(AF_INET, &ClientAddr.sin_addr, Addr, sizeof(Addr));
    printf("\n[SERVER] Client Connected : IP 주소 = %s, 포트 번호=%d\n", Addr, ntohs(ClientAddr.sin_port));
#endif 


    char clientIP[MAX_IP_LEN] = { 0, };
    inet_ntop(AF_INET, &(ClientAddr.sin_addr), clientIP, MAX_IP_LEN - 1);


    Result.Result = TResult::SUCCESS;
    Result.IP     = clientIP;
    Result.SetSocket(ClientSock);

    return Result;
}

void TCPListenNetwork::InsertSocket(SOCKET& socket)
{
}


