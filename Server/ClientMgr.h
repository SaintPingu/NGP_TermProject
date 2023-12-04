#pragma once
// 2023-11-06 (장재문) : 틀을 잡기 위해 미리 생성함. 네트워크를 통해 들어온 클라이언트들을 관리하는 매니저. - 향후 동기화에 주의하자. 
// 2023-11-10-FRI (장재문) : 세부내용 작성 
// 2023-11-15-WED (장재문) : 클라이언트 접속 관련 동기화 처리 및 접속 종료 처리 구현  

#include "ServerPacket.h"
#include "ServerNetwork.h"


/// +------------------
///	    CLIENT MGR
/// ------------------+

enum class mutexType
{
	/// -clientPool 에 접근하는 함수는 mutex[accessClientPool]를 꼭 lock 걸어야합니다.
	accessClientPool,	// 클라이언트 정보 접근 동기화 뮤텍스
	createID,			// 클라이언트 정보 접근 ID 생성 뮤텍스
	terminateID_event,	// 클라이언트 접속 종료 처리 동기화 뮤텍스
	END,
};

enum class clientEventType
{
	terminateID,
	registerNewID,
	END,
};
constexpr int ALL_CLIENT = -9;
class ClientInfo;
class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);

private:
	std::vector<ClientInfo*>	clientPool;							// 접속 클라이언트 관리 
	int							clientPoolIndex{};					// 현재 클라이언트 풀의 최대 index
    
	Mutex						mutex[(UINT)mutexType::END]{};		// 뮤텍스 관리

	std::queue<std::pair<clientEventType, PVOID>>	clientEvents;

	PacketGenerator				packetGenerator{};
	PacketLoader				packetLoader{};

	std::vector<ClientInfo*> GetAllClients();
public:
	bool	Event();
	bool	SendPacket();

	bool	SetPacketBuffer();

	TResult CreateClientThread(int ID);


	TResult ProcessCommand();			// 3. 패킷 해석		- 클라이언트로부터 받은 패킷을 해석하고 데이터 업데이트 


	// 접속된 클라이언트의 아이디와 TResult 값을 반환한다.
	std::pair<int, TResult> RegisterConnectedClient(std::string clientIP, SOCKET& sock); // 접속된 클라이언트 ClientPool 에 등록 ( 해당 인덱스는 곧 본인의 ID ) 
	// 접속 종료된 클라이언트들을 처리한다 ( 이벤트 처리 )
	void RegisterTerminateClientID(int id); // 접속 종료 아이디 이벤트 등록 - 클라이언트 쓰레드에서 접속이 종료된 것을 클라이언트 매니저에게 알린다. 이를 이벤트 처리한다. 
	void Disconnect(int id);				// clientID에 해당하는 ClientInfo를 중단한다.
	
	int GetPoolIndex() const { return clientPoolIndex; }
	ClientInfo* GetClient(int ID);


public:
	bool Init();
	void Stop(int id);
	void Stop();
	void Exit(int id);
	void Exit();

	void PushCommand(int clientID, BYTE cmd, void* data, size_t size);
	void ClearCommand(int clientID);
private:
	int CreateID();


public:
	ClientMgr() {};
	~ClientMgr();


};

