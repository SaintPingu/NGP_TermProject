#pragma once


// 2023-11-20-MON (장재문) - 최대한 클라이언트 쪽 코드 변경 및 추가를 막기 위해서 싱글톤으로 생성함



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

// 2023-11-20-MON (장재문) - 클라이언트 네트워크 매니저 가져오기 define 정의 
#define CLIENT_NETWORK_MGR ClientNetMgr::Inst()
#define CLIENT_NETWORK ClientNetMgr::Inst()->GetClientNetwork()
#define PACKET_BUFFER CLIENT_NETWORK->GetPacketBuffer();
