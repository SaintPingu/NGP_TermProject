#include "stdafx.h"
#include "ClientMgr.h"
#include "ClientInfo.h"
#include "ServerFramework.h"
#include "SceneMgr.h"
#include "LobbyScene.h"
#include "StageScene.h"
#include "BattleScene.h"


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

			//12-03 민동현 : SCENE_MGR에서 클라이언트가 위치한 씬에서 해당 클라이언트를 제거하도록 한다.
			SCENE_MGR->DeleteClient(ID);

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


bool ClientMgr::SetPacketBuffer()
{
	packetLoader.Clear();

	for (int i = 0; i < GetPoolIndex(); ++i)
	{
		auto& client = clientPool[i];
		if (client == nullptr) {
			continue;
		}

		if (client->GetConnectFlag() == ConnectFlag::RecvFinish) {
			packetLoader.SetPacketBuffer(client->GetID(), &client->GetPacketBuffer());
		}
	}

	return true;
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
	packetGenerator.GenerateData();

	// Send
	for (auto& [clientID, packetBuffer] : packetLoader.GetReceivedPacketBuffers()) {

		auto& client = clientPool[clientID];
		if (client == nullptr) {
			continue;
		}
		
		DataType dataType = GetDataType(client);
		// 패킷 생성에 성공했다면 전송
		if (packetGenerator.GeneratePacket(client->GetPacketBuffer(), client->GetCmdList(), dataType)) {
			client->Send();
		}
	}

	packetLoader.Clear();
	packetGenerator.DeleteData();

	return true;
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
		SCENE_MGR->DeleteClient(id);
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

void ClientMgr::PushCommand(int clientID, BYTE cmd, void* data, size_t size)
{
	ClientInfo* client = GetClient(clientID);
	if (client) {
		client->PushCommand(cmd, data, size);
	}
	else {
		std::cout << "[ERROR - PushCommand ] Client ID doesn't exist :: [" << clientID << "]\n";
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

void ClientMgr::Disconnect(int id)
{
	ClientInfo* client = GetClient(id);
	if (client) {
		client->Disconnect();
	}
}

ClientInfo* ClientMgr::GetClient(int ID)
{
	//	std::lock_guard<Mutex> lock(mutex[(UINT)mutexType::accessClientPool]);
	return clientPool[ID];
}

TResult ClientMgr::ProcessCommand()
{
	Command cmd{};
	PacketBuffer data;
	while (true) {
		int clientID = packetLoader.PopCommand(cmd, data);
		if (clientID == -1) {
			break;
		}
		else if (clientID == -2) {
			continue;
		}

		switch (SCENE_MGR->GetClientLocation(clientID)) {
		case SceneType::Lobby:
			SCENE_MGR->Lobby()->ProcessCommand(clientID, cmd, &data);	// data is nullptr in lobby
			break;
		case SceneType::Stage:
			SCENE_MGR->Stage()->ProcessCommand(clientID, cmd, &data);
			break;
		case SceneType::Battle:
			SCENE_MGR->Battle()->ProcessCommand(clientID, cmd, &data);
			break;
		default:
			SCENE_MGR->DeleteClient(clientID);	// [ERROR] 클라이언트가 위 씬에 존재하지 않음
			break;
		}
	}
	return TResult();
}

std::pair<int, TResult> ClientMgr::RegisterConnectedClient(std::string clientIP, SOCKET& sock)
{
	int id = CreateID();
	if (id == invalidID)
		return std::pair<int, TResult>(invalidID, TResult::CLIENT_CAN_NOT_ACCEPT_ANYMORE);

	ClientInfo* NewUser = new ClientInfo;
	ServerNetwork* serverNet = new ServerNetwork();
	serverNet->SetSocket(sock);

	NewUser->SetID(id);
	NewUser->SetServerNet(serverNet);

	clientEventType type = clientEventType::registerNewID;
	clientEvents.push(std::make_pair(type, NewUser));

	++clientPoolIndex;

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
