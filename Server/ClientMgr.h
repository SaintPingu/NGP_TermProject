#pragma once
// 2023-11-06 (���繮) : Ʋ�� ��� ���� �̸� ������. ��Ʈ��ũ�� ���� ���� Ŭ���̾�Ʈ���� �����ϴ� �Ŵ���. - ���� ����ȭ�� ��������. 
// 2023-11-10-FRI (���繮) : ���γ��� �ۼ� 

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
	std::vector<ClientInfo>		ClientPool;		// ���� Ŭ���̾�Ʈ ���� 
    PacketGenerator				PacketGen;		// ��Ŷ ����
	
	Mutex						clientRegisterMutex; // Ŭ���̾�Ʈ ��� ����ȭ ó���� ���ؽ�

public:

	bool SetPacketBuffer();
	bool SendPacket();
	bool CheckInsertedSocket(); 
	void PushCommand();

	TResult RegisterConnectedClient(std::string clientIP, SOCKET& sock); // ���ӵ� Ŭ���̾�Ʈ ClientPool �� ��� ( �ش� �ε����� �� ������ ID ) 
	TResult CreateClientThread(SOCKET& sock, int ID);


public:
	bool Init();

private:
	int CreateID();


public:
	ClientMgr() {};
	~ClientMgr();


};

