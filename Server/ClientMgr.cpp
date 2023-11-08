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
