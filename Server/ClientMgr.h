#pragma once
// 2023-11-06 (���繮) : Ʋ�� ��� ���� �̸� ������. ��Ʈ��ũ�� ���� ���� Ŭ���̾�Ʈ���� �����ϴ� �Ŵ���. - ���� ����ȭ�� ��������. 
// 2023-11-10-FRI (���繮) : ���γ��� �ۼ� 
// 2023-11-15-WED (���繮) : Ŭ���̾�Ʈ ���� ���� ����ȭ ó�� �� ���� ���� ó�� ����  

#include "ServerPacket.h"
#include "ServerNetwork.h"


/// +------------------
///	    CLIENT MGR
/// ------------------+

enum class mutexType
{
	accessClientPool,	// Ŭ���̾�Ʈ ���� ���� ����ȭ ���ؽ�
	createID,			// Ŭ���̾�Ʈ ���� ���� ID ���� ���ؽ�
	terminateID_event,	// Ŭ���̾�Ʈ ���� ���� ó�� ����ȭ ���ؽ�
	END,
};

class ClientInfo;
class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);

private:
	/// -clientPool �� �����ϴ� �Լ��� mutex[accessClientPool]�� �� lock �ɾ���մϴ�.
	//std::vector<ClientInfo*>	newClients;						// �ű� ���� Ŭ���̾�Ʈ
	std::vector<ClientInfo*>	clientPool;						// ���� Ŭ���̾�Ʈ ���� 
	int							clientPoolIndex{};				// ���� Ŭ���̾�Ʈ Ǯ�� �ִ� index
    
	PacketGenerator				packetGen;						// ��Ŷ ����
	Mutex						mutex[(UINT)mutexType::END]{};	// ���ؽ� ����

	std::queue<int>				terminatedID_events;

public:

	bool SetPacketBuffer();
	bool SendPacket();
	void InsertNewSocket();
	void PushCommand();

	// ���ӵ� Ŭ���̾�Ʈ�� ���̵�� TResult ���� ��ȯ�Ѵ�.
	std::pair<int, TResult> RegisterConnectedClient(std::string clientIP, SOCKET& sock); // ���ӵ� Ŭ���̾�Ʈ ClientPool �� ��� ( �ش� �ε����� �� ������ ID ) 
	TResult CreateClientThread(SOCKET& sock, int ID);

	// ���� ����� Ŭ���̾�Ʈ���� ó���Ѵ� ( �̺�Ʈ ó�� )
	void RegisterTerminateClientID(int id); // ���� ���� ���̵� �̺�Ʈ ��� - Ŭ���̾�Ʈ �����忡�� ������ ����� ���� Ŭ���̾�Ʈ �Ŵ������� �˸���. �̸� �̺�Ʈ ó���Ѵ�. 
	void ExecuteTerminateIdEvents();
	int GetPoolIndex() const { return clientPoolIndex; }


public:
	bool Init();
	void Stop(int id);
	void Stop();
	void Exit(int id);
	void Exit();



private:
	int CreateID();


public:
	ClientMgr() {};
	~ClientMgr();


};

