#include "stdafx.h"
#include "ClientMgr.h"

SINGLETON_PATTERN_DEFINITION(ClientMgr);

void ClientMgr::CreateClientThread()
{
}

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
	// 동기화 처리 주의 ..업데이트 예정.. 

	int id = ClientMgr::CreateID();
	if (id == -1)
		return TResult::MAX_CLIENT_CAN_NOT_ACCEPT_ANYMORE_NOT_ERROR;

	ClientInfo NewUser{};
	NewUser.ID        = id;
	NewUser.ServerNet = new ServerNetwork;
	
	ClientPool[NewUser.ID] = NewUser;
	return TResult::SUCCESS;
}

TResult ClientMgr::ExecuteClientThread(SOCKET& sock)
{
	return TResult();
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
	for (int i = 0; i < ClientPool.size(); ++i)
	{
		ClientPool.clear();
	}

}
