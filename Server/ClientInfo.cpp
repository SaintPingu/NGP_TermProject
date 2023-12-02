#include "stdafx.h"
#include "ClientInfo.h"
#include "ServerNetwork.h"
#include "ServerFramework.h"
#include "ClientMgr.h"
#include "SceneMgr.h"

ClientInfo::ClientInfo()
{
	executeLogic = true;
	SetConnectFlag(ConnectFlag::RecvFinish);
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
	std::cout << "\n\t\t-> Client [" << ID << "] 구동 중...\n";

	// 2023-11-26 (민동현) : 첫 패킷은 클라이언트의 ID를 전송한다.
	serverNet->SendClientID(ID);

	// 2023-11-16-TUR (민동현) : 리시브 완료 시 외부에서 "Send() { SetEvent(sendEvent) }" 를 호출해 전송하도록 한다.
	while(executeLogic)
	{
		//std::cout << "\t\t-> Client [" << ID << "] 송신 대기...\r";

		WaitForSingleObject(sendEvent, INFINITE);
		if (isDisconnected) {	// 연결 종료 시 종료
			break;
		}

		curConnectFlag = ConnectFlag::SendStart;		// 전송 대기
		if (SendPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::SendFinish;

		//std::cout << "\t\t-> Client [" << ID << "] 송신 완료 및 수신 대기\n";

		curConnectFlag = ConnectFlag::RecvStart;
		if (RecvPacket() == TResult::FAIL) {
			break;
		}
		curConnectFlag = ConnectFlag::RecvFinish;

		//std::cout << "\t\t-> Client [" << ID << "] 수신 완료\n";

		ResetEvent(sendEvent);
	}

	serverNet->SendTerminatePacket();
	CLIENT_MGR->RegisterTerminateClientID(ID); // 클라이언트 매니저에게 자신이 종료 됐다는 것을 알린다. 이후 이벤트 처리 할 것임 
	std::cout << "\n\t\t-> Client [" << ID << "] 종료... -> ClientMgr 에서 이벤트 처리\n";

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

void ClientInfo::PushCommand(BYTE& cmd, void* data, size_t size)
{
	cmdList.PushCommand(cmd, data, size);
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



