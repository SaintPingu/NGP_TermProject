#pragma once
// 2023-11-06 (장재문) : 틀을 잡기 위해 미리 생성함. 네트워크를 통해 들어온 클라이언트들을 관리하는 매니저. - 향후 동기화에 주의하자. 
// 2023-11-10-FRI (장재문) : 세부내용 작성 

#include "ServerPacket.h"
#include "ServerNetwork.h"

struct ClientInfo
{
	int					ID = -1;
	ServerNetwork*		ServerNet{};

	bool IsConnected()	{ return ServerNet->GetSocket() != 0 ? true : false; }
	void Clear()		{ ID = -1; if(ServerNet != nullptr and ServerNet->GetSocket() != 0) ServerNet->CloseSocket(); ServerNet = nullptr; }
	void ClientThreadLogic()
	{

	}

};

class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);

private:
	std::vector<ClientInfo>		ClientPool;		// 접속 클라이언트 관리 
    PacketGenerator				PacketGen;		// 패킷 관리
	
	Mutex						clientRegisterMutex; // 클라이언트 등록 동기화 처리용 뮤텍스

public:

	bool SetPacketBuffer();
	bool SendPacket();
	bool CheckInsertedSocket(); 
	void PushCommand();

	TResult RegisterConnectedClient(std::string clientIP, SOCKET& sock); // 접속된 클라이언트 ClientPool 에 등록 ( 해당 인덱스는 곧 본인의 ID ) 
	TResult CreateClientThread(SOCKET& sock, int ID);


public:
	bool Init();

private:
	int CreateID();


public:
	ClientMgr() {};
	~ClientMgr();


};

