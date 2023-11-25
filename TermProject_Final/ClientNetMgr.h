#pragma once


// 2023-11-20-MON (���繮) - �ִ��� Ŭ���̾�Ʈ �� �ڵ� ���� �� �߰��� ���� ���ؼ� �̱������� ������



class ClientNetwork;
class ClientNetMgr {
	SINGLETON_PATTERN(ClientNetMgr);

private:
	ClientNetwork*	clientNet{};


public:
	TResult Init(short portnum);
	void Execute();

	ClientNetwork* GetClientNetwork() { return clientNet; }


	ClientNetMgr();
	~ClientNetMgr();

};

// 2023-11-20-MON (���繮) - Ŭ���̾�Ʈ ��Ʈ��ũ �Ŵ��� �������� define ���� 
#define CLIENT_NETWORK_MGR ClientNetMgr::Inst()
#define CLIENT_NETWORK ClientNetMgr::Inst()->GetClientNetwork()
#define PACKET_BUFFER CLIENT_NETWORK->GetPacketBuffer();
