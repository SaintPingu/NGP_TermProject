
#include "stdafx.h"
#include "ClientNetMgr.h"
#include "ClientNetwork.h"

SINGLETON_PATTERN_DEFINITION(ClientNetMgr);

ClientNetMgr::ClientNetMgr()
{
}

ClientNetMgr::~ClientNetMgr()
{


}

void ClientNetMgr::Execute()
{
	if (Init(9000) == TResult::SUCCESS)
	{
		clientNet->Logic();
	}
}

TResult ClientNetMgr::Init(short portnum)
{
	// �ܼ�â ���� 
	AllocConsole();
	// ǥ�� �Է�, ��� �� ������ �ַܼ� �����մϴ�.
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
	std::string serverIP{};
	std::cout << "���� ������ �ּ� �Է� : ";
	std::cin >> serverIP;

	// Ŭ���̾�Ʈ ��Ʈ��ũ �ʱ�ȭ 
	clientNet = new ClientNetwork{};
	result = clientNet->Init(serverIP, portnum);
	if (result == TResult::FAIL)
		return TResult::FAIL;


	return result;
}



