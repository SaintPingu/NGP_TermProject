#include "stdafx.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "ServerFramework.h"
#include "SceneMgr.h"

SINGLETON_PATTERN_DEFINITION(ClientMgr);
constexpr int invalidID = -1;

bool ClientMgr::Event()
{
	// 동기화 처리  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);

	while (!clientEvents.empty())
	{
		std::pair<clientEventType, PVOID> event = clientEvents.front();
		clientEvents.pop();

		switch (event.first)
		{
		case clientEventType::terminateID:
		{
			int  ID = *(int*)event.second;
			TResult result = clientPool[ID]->Clear();
			if (result == TResult::SUCCESS)
			{
				SAFE_DELETE(clientPool[ID]);
			}
			SAFE_DELETE(event.second);
		}
			break;
		case clientEventType::registerNewID:
		{
			ClientInfo* newUser   = (ClientInfo*)event.second;
			int			ID        = newUser->GetID();
			clientPool[ID]        = newUser;
			CLIENT_MGR->CreateClientThread(ID);
		}
			break;
		}
	}


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
	// Generate
	packetGen.GenerateData();

	// Send
	for (int i = 0; i < GetPoolIndex() + 1; ++i)
	{
		auto& client = clientPool[i];
		if (client == nullptr) {
			continue;
		}

		if (client->GetConnectFlag() == ConnectFlag::RecvFinish) {
			DataType dataType = GetDataType(client);
			packetGen.GeneratePacket(client->GetPacketBuffer(), client->GetCmdList(), dataType);
			client->Send();
		}
	}

	return true;
}

TResult ClientMgr::Update()
{
	ProcessCommand();

	return TResult();
}

TResult ClientMgr::SetPacketBuffer(int ID)
{
	// 동기화 처리  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);

	if (clientPool[ID] == nullptr)
	{
		CRASH("INVALID ID");
		return TResult::FAIL;
	}

	
	PacketBuffer buf = clientPool[ID]->GetPacketBuffer();
	packetLoader.SetPacketBuffer(ID, &buf);

	return TResult();
}


TResult ClientMgr::ProcessCommand()
{
	BYTE			cmd{};
	PacketBuffer	buf{};
	
	while (packetLoader.PopCommand(cmd, buf) != -1)
	{
		// TODO : Process Command 클라이언트 처리  

		break;
	}
	

	return TResult();
}


TResult ClientMgr::CreateClientThread(int ID)
{
	if (ID == invalidID or clientPool[ID] == nullptr)
	{
		CRASH("INVALID CLIENT ID - CreateClientThread()");
		return TResult::CLIENT_NOT_CONNECTED;
	}

	clientPool[ID]->CreateThread();
	
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
		ClientInfo 에서 접속이 종료된 아이디를 처리하기 위해 이벤트 처리합니다.  
		여러 클라이언트가 동시에 접속 종료를 알릴 수 있으니 동기화 처리해야한다.
	*/
	// 동기화 처리  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::terminateID_event]);
	
	int*			terminateID = new int(id);
	clientEventType type        = clientEventType::terminateID;
	clientEvents.push(std::make_pair(type, terminateID));
}

std::pair<int, TResult> ClientMgr::RegisterConnectedClient(std::string clientIP, SOCKET& sock)
{
	int id = CreateID();
	if (id == invalidID)
		return std::pair<int, TResult>(invalidID, TResult::CLIENT_CAN_NOT_ACCEPT_ANYMORE);

	ClientInfo* NewUser = new ClientInfo;
	NewUser->SetID(id);
	NewUser->SetServerNet(new ServerNetwork);

	clientEventType type = clientEventType::registerNewID;
	clientEvents.push(std::make_pair(type, NewUser));

	return std::pair<int, TResult>(id, TResult::SUCCESS);
}

int ClientMgr::CreateID()
{	
	// 동기화 처리  
	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::createID]);

	// 순차탐색
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
