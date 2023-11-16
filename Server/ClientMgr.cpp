#include "stdafx.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "ServerFramework.h"
#include "SceneMgr.h"

SINGLETON_PATTERN_DEFINITION(ClientMgr);
constexpr int invalidID = -1;

bool ClientMgr::SetPacketBuffer()
{

	return false;
}

DataType GetDataType(ClientInfo* client)
{
	switch (SCENE_MGR->GetClientLocation(client->GetID())) {
	case SceneType::Lobby:
		return DataType::Lobby;
	case SceneType::Stage:
		return DataType::Stage;
	case SceneType::Battle:
		return DataType::Battle;
	}

	assert(0);
	return DataType::Lobby;
}

bool ClientMgr::SendPacket()
{
	//InsertNewSocket();

	// Generate
	packetGen.GenerateData();

	// Send
	for (int i = 0; i < GetPoolIndex() + 1; ++i)
	{
		auto& client = clientPool[i];
		if (client == nullptr) {
			continue;
		}

		if (client->GetConnectFlag() == ConnectFlag::recv) {
			DataType dataType = GetDataType(client);
			packetGen.GeneratePacket(client->GetPacketBuffer(), client->GetCmdList(), dataType);
			client->Send();
		}
	}

	return true;
}

void ClientMgr::InsertNewSocket()
{
	//for (auto& client : newClients) {
	//	clientPool[client->GetID()] = client;
	//}
	//newClients.clear();
}

void ClientMgr::PushCommand()
{

}

std::pair<int, TResult> ClientMgr::RegisterConnectedClient(std::string clientIP, SOCKET& sock)
{
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);

	int id = CreateID();
	if (id == invalidID)
		return std::pair<int, TResult>(invalidID, TResult::CLIENT_CAN_NOT_ACCEPT_ANYMORE);

	ClientInfo* NewUser = new ClientInfo;
	NewUser->SetID(id);
	NewUser->SetServerNet(new ServerNetwork);

	clientPool[id] = NewUser;
	return std::pair<int, TResult>(id, TResult::SUCCESS);
}


TResult ClientMgr::CreateClientThread(SOCKET& sock, int ID)
{
	if (clientPool[ID] == nullptr or ID == invalidID)
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
	constexpr int maxClientSize = 256;

	clientPool.resize(maxClientSize);
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
		if (clientPool[i] == nullptr) {
			if (clientPoolIndex < i) {
				clientPoolIndex = i;
			}
			return i;
		}
	}
	return invalidID;
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
