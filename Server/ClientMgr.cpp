#include "stdafx.h"
#include "ClientMgr.h"

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

TResult ClientMgr::RegisterConnectedClient(std::string clientIP, SOCKET& sock)
{
	// ����ȭ ó��  
	std::lock_guard<Mutex> lock(clientRegisterMutex);

	int id = ClientMgr::CreateID();
	if (id == -1)
		return TResult::CLIENT_CAN_NOT_ACCEPT_ANYMORE;

	ClientInfo NewUser	= {};
	NewUser.ID			= id;
	NewUser.ServerNet	= new ServerNetwork;
	
	ClientPool[NewUser.ID] = NewUser;
	return TResult::SUCCESS;
}


TResult ClientMgr::CreateClientThread(SOCKET& sock, int ID)
{
	// Ŭ���̾�Ʈ Thread ���� 
	std::thread Client([&]() {
		/* Ŭ���̾�Ʈ ���� �Լ� */ClientPool[ID].ClientThreadLogic();
		});
	Client.join();

	return TResult::CLIENT_DISCONNECT;
}



bool ClientMgr::Init()
{
	
	ClientPool.resize(100);


	return true;
}

int ClientMgr::CreateID()
{
	for (int i = 0; i < ClientPool.size(); ++i)
	{
		if (ClientPool[i].ID == -1)
			return i;
	}
	return -1;
}

ClientMgr::~ClientMgr()
{
	//ClientPool.clear();
	//for (int i = 0; i < ClientPool.size(); ++i)
	//{
	//	//ClientPool[i].Clear();
	//}

}
