#include "stdafx.h"
#include "ClientInfo.h"
#include "ServerNetwork.h"
#include "ServerFramework.h"
#include "ClientMgr.h"
#include "SceneMgr.h"

ClientInfo::ClientInfo()
{
	executeLogic = true;
	SetConnectFlag(ConnectFlag::recv);
	sendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

ClientInfo::~ClientInfo()
{
	if(thread.joinable())
		thread.join();
}


void ClientInfo::CreateThread()
{
	thread = std::thread([=]() {this->Logic(); });
}

TResult ClientInfo::Logic()
{
	std::cout << "\n\t\t-> Client [" << ID << "] ���� ��...\n";

// 2023-11-16-TUR (�ε���) : ���ú� �Ϸ� �� �ܺο��� "Send() { SetEvent(sendEvent) }" �� ȣ���� �����ϵ��� �Ѵ�.
	while(executeLogic)
	{
		WaitForSingleObject(sendEvent, INFINITE);

		//std::cout << "\t\t-> Client [" << ID << "] �۽� ���...\r";
		curConnectFlag = ConnectFlag::send;		// ���� ���
		SendPacket();

		//std::cout << "\t\t-> Client [" << ID << "] �۽� �Ϸ� �� ���� ���\n";

		RecvPacket();
		curConnectFlag = ConnectFlag::recv;		// ���ú� �Ϸ�

		//std::cout << "\t\t-> Client [" << ID << "] ���� �Ϸ�\n";

		ResetEvent(sendEvent);
	}

	CLIENT_MGR->RegisterTerminateClientID(ID); // Ŭ���̾�Ʈ �Ŵ������� �ڽ��� ���� �ƴٴ� ���� �˸���. ���� �̺�Ʈ ó�� �� ���� 
	std::cout << "\n\t\t-> Client [" << ID << "] ����... -> ClientMgr ���� �̺�Ʈ ó��\n";

	return TResult::CLIENT_DISCONNECT;
}

TResult ClientInfo::RecvPacket()
{
	return serverNet->RecvPacket();
}

TResult ClientInfo::SendPacket()
{
	return serverNet->SendPacket();
}

void ClientInfo::SetID(int id)
{
	ID = id;
	SCENE_MGR->InsertClient(ID);
}

PacketBuffer& ClientInfo::GetPacketBuffer()
{
	return serverNet->GetPacketBuffer();
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



