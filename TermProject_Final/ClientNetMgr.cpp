#include "stdafx.h"
#include "ClientNetMgr.h"
#include "ClientNetwork.h"
#include "framework.h"

SINGLETON_PATTERN_DEFINITION(ClientNetMgr);

ClientNetMgr::ClientNetMgr()
{
}

ClientNetMgr::~ClientNetMgr()
{


}

void ClientNetMgr::Execute()
{
	if (Init(9000) == TResult::SUCCESS) {
		clientNet->Logic();
	}
	else {
		// ���� ���� ����
		framework->WakeForPacket();
	}
}

TResult ClientNetMgr::Init(short portnum)
{
	AllocConsole();

	FILE* consoleIn, * consoleOut, * consoleErr;
	freopen_s(&consoleIn, "CONIN$", "r", stdin);
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
	freopen_s(&consoleErr, "CONOUT$", "w", stderr);

	TResult result = TResult::SUCCESS;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return TResult::FAIL;

	// 1. ���� ������ �ּ� �Է� 
	const char* serverIP = "127.0.0.1";

	// Ŭ���̾�Ʈ ��Ʈ��ũ �ʱ�ȭ 
	clientNet = std::make_shared<ClientNetwork>();
	result = clientNet->Init(serverIP, portnum);
	if (result == TResult::FAIL)
		return TResult::FAIL;


	return result;
}



