#include "stdafx.h"
#include "ClientMgr.h"
#include "ClientInfo.h"

SINGLETON_PATTERN_DEFINITION(ClientMgr);

bool ClientMgr::SetPacketBuffer()
{

	return false;
}

bool ClientMgr::SendPacket()
{

	return false;
}

bool ClientMgr::CheckInsertedSocket()
{

	return false;
}

void ClientMgr::PushCommand()
{

}

std::pair<int, TResult> ClientMgr::RegisterConnectedClient(std::string clientIP, SOCKET& sock)
{
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);

	int id = ClientMgr::CreateID();
	if (id == -1)
		return std::pair<int, TResult>(-1, TResult::CLIENT_CAN_NOT_ACCEPT_ANYMORE);

	ClientInfo* NewUser = new ClientInfo;
	NewUser->SetID(id);
	NewUser->SetServerNet(new ServerNetwork);

	clientPool[id] = NewUser;
	return std::pair<int, TResult>(id, TResult::SUCCESS);
}


TResult ClientMgr::CreateClientThread(SOCKET& sock, int ID)
{
	if (clientPool[ID] == nullptr or ID == -1)
	{
		CRASH("INVALID CLIENT ID - CreateClientThread()");
		return TResult::CLIENT_NOT_CONNECTED;
	}

	// Ŭ���̾�Ʈ Thread ���� 
	std::thread Client([&]() {
		clientPool[ID]->Logic();
		});
	Client.join();

	return TResult::CLIENT_DISCONNECT;
}



bool ClientMgr::Init()
{

	clientPool.resize(100);
	std::fill(clientPool.begin(), clientPool.end(), nullptr);

	return true;
}

void ClientMgr::Stop(int id)
{
	if(clientPool[id] != nullptr)
	{
		clientPool[id]->Stop();
	}
}

void ClientMgr::Stop()
{
	for (ClientInfo* info : clientPool)
	{
		if (info != nullptr)
			info->Stop();
	}
}

void ClientMgr::Exit(int id)
{
	if (clientPool[id] != nullptr)
	{
		clientPool[id]->Exit();
	}
}

void ClientMgr::Exit()
{
	for (ClientInfo* info : clientPool)
	{
		if (info != nullptr)
			info->Exit();
	}
}

void ClientMgr::RegisterTerminateClientID(int id)
{
	/*
		ClientInfo ���� ������ ����� ���̵� ó���ϱ� ���� �̺�Ʈ ó���մϴ�.  
		���� Ŭ���̾�Ʈ�� ���ÿ� ���� ���Ḧ �˸� �� ������ ����ȭ ó���ؾ��Ѵ�.
	*/
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::terminateID_event]);
	terminatedID_events.push(id);

}

void ClientMgr::ExecuteTerminateIdEvents()
{
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);

	while (!terminatedID_events.empty())
	{
		int ID = terminatedID_events.front();
		terminatedID_events.pop();

		TResult result = clientPool[ID]->Clear();
		if(result == TResult::SUCCESS)
			clientPool[ID] = nullptr;
	}
	
}

int ClientMgr::CreateID()
{	
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::createID]);

	// ����Ž��
	for (int i = 0; i < clientPool.size(); ++i)
	{
		if (clientPool[i] == nullptr)
			return i;
	}
	return -1;
}

ClientMgr::~ClientMgr()
{
	for (int i = 0; i < clientPool.size(); ++i)
	{
		if (clientPool[i] != nullptr)
		{
			clientPool[i]->Clear();
			clientPool[i] = nullptr;
		}
	}

}
