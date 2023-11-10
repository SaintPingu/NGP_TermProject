#pragma once
// 2023-11-06 (���繮) : Ʋ�� ��� ���� �̸� ������. ��Ʈ��ũ�� ���� ���� Ŭ���̾�Ʈ���� �����ϴ� �Ŵ���. - ���� ����ȭ�� ��������. 
// 2023-11-10-FRI (���繮) : ���γ��� �ۼ� 

#include "ServerPacket.h"
#include "ServerNetwork.h"

struct ClientInfo
{
	int				ID = -1;
	ServerNetwork*  ServerNet{};

	bool IsConnected()	{ return ServerNet->GetSocket() != 0 ? true : false; }
	void Clear()		{ ID = -1; if(ServerNet->GetSocket() != 0) ServerNet->CloseSocket(); ServerNet = nullptr; }
};

class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);

private:
	std::vector<ClientInfo>		ClientPool;		// ���� Ŭ���̾�Ʈ ���� 
    PacketGenerator				PacketGen;		// ��Ŷ ����
	Mutex						Mute;			// ����ȭ ó�� 

public:
	void CreateClientThread();
	bool SetPacketBuffer();
	bool SendPacket();
	bool CheckInsertedSocket(); 
	void PushCommand();

	TResult RegisterConnectedClient(std::string clientIP, SOCKET& sock); // ���ӵ� Ŭ���̾�Ʈ ClientPool �� ��� ( �ش� �ε����� �� ������ ID ) 
	TResult ExecuteClientThread(SOCKET& sock);


public:
	bool Init();

private:
	int CreateID();


public:
	ClientMgr() {};
	~ClientMgr();


};

