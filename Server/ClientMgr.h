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
	/// -clientPool �� �����ϴ� �Լ��� mutex[accessClientPool]�� �� lock �ɾ���մϴ�.
	accessClientPool,	// Ŭ���̾�Ʈ ���� ���� ����ȭ ���ؽ�
	createID,			// Ŭ���̾�Ʈ ���� ���� ID ���� ���ؽ�
	terminateID_event,	// Ŭ���̾�Ʈ ���� ���� ó�� ����ȭ ���ؽ�
	END,
};

enum class clientEventType
{
	terminateID,
	registerNewID,
	END,
};

class ClientInfo;
class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);

private:
	std::vector<ClientInfo*>	clientPool;							// ���� Ŭ���̾�Ʈ ���� 
	int							clientPoolIndex{};					// ���� Ŭ���̾�Ʈ Ǯ�� �ִ� index
    
	PacketGenerator				packetGen;							// ��Ŷ ����
	Mutex						mutex[(UINT)mutexType::END]{};		// ���ؽ� ����

	std::queue<std::pair<clientEventType, PVOID>>	clientEvents;	// eventtype, data type

	PacketGenerator				packetGenerator{};
	PacketLoader				packetLoader{};

public:
	bool	Event();
	TResult Update();
	bool	SendPacket();

	TResult CreateClientThread(int ID);

	TResult SetPacketBuffer(int ID);	// 2. ��Ŷ ����		- ID Ŭ���̾�Ʈ�� ��Ŷ�� �����Ѵ�. 
	TResult ProcessCommand();			// 3. ��Ŷ �ؼ�		- Ŭ���̾�Ʈ�κ��� ���� ��Ŷ�� �ؼ��ϰ� ������ ������Ʈ 


	// ���ӵ� Ŭ���̾�Ʈ�� ���̵�� TResult ���� ��ȯ�Ѵ�.
	std::pair<int, TResult> RegisterConnectedClient(std::string clientIP, SOCKET& sock); // ���ӵ� Ŭ���̾�Ʈ ClientPool �� ��� ( �ش� �ε����� �� ������ ID ) 
	// ���� ����� Ŭ���̾�Ʈ���� ó���Ѵ� ( �̺�Ʈ ó�� )
	void RegisterTerminateClientID(int id); // ���� ���� ���̵� �̺�Ʈ ��� - Ŭ���̾�Ʈ �����忡�� ������ ����� ���� Ŭ���̾�Ʈ �Ŵ������� �˸���. �̸� �̺�Ʈ ó���Ѵ�. 

	/// +-------------
	///	    G E T 
	/// -------------+
	PacketGenerator		GetPacketGenerator()	const	{	return packetGenerator;		}
	PacketLoader		GetPacketLoader()		const	{	return packetLoader;		}
	int					GetPoolIndex()			const	{	return clientPoolIndex;		}

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

