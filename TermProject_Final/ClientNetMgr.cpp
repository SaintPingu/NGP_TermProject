
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


TResult ClientNetMgr::Init(short portnum)
{
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

