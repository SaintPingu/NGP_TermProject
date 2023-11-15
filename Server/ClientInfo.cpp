#include "stdafx.h"
#include "ClientInfo.h"
#include "ServerNetwork.h"
#include "ServerFramework.h"
#include "ClientMgr.h"

ClientInfo::ClientInfo()
{

}

ClientInfo::~ClientInfo()
{

}


TResult ClientInfo::Logic()
{
	std::cout << "\t\t-> Client [" << ID << "] ���� ��...\n";

	while(executeLogic)
	{
		if (active == false)
			continue;

		switch (curConnectFlag)
		{
		case ConnectFlag::recv: 
		{
			RecvPacket();
		}
			break;
		case ConnectFlag::send: 
		{
			SendPacket();
		}
			break;
		}

	}

	CLIENT_MGR->RegisterTerminateClientID(ID); // Ŭ���̾�Ʈ �Ŵ������� �ڽ��� ���� �ƴٴ� ���� �˸���. ���� �̺�Ʈ ó�� �� ���� 
	std::cout << "\t\t-> Client [" << ID << "] ����... -> ClientMgr ���� �̺�Ʈ ó��\n";

	return TResult::CLIENT_DISCONNECT;
}

TResult ClientInfo::RecvPacket()
{


	return TResult();
}

TResult ClientInfo::SendPacket()
{


	return TResult();
}

bool ClientInfo::IsConnected()
{
	if (serverNet->IsTerminate() == true)
		return false;
	else
		return serverNet->GetSocket() != 0 ? true : false;

}

TResult ClientInfo::Clear()
{
	ID                       = -1; 
	TResult result           = TResult::SUCCESS;	

	if (serverNet != nullptr and serverNet->GetSocket() != 0)
	{
		result = serverNet->CloseSocket();
	}
	if (result == TResult::SUCCESS)
	{
		serverNet = nullptr;
	}

	return result;
}



