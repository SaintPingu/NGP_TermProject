
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

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return TResult::FAIL;

	// 1. 서버 아이피 주소 입력 
	std::string serverIP{};
	std::cout << "서버 아이피 주소 입력 : ";
	std::cin >> serverIP;

	// 클라이언트 네트워크 초기화 
	clientNet = new ClientNetwork{};
	result = clientNet->Init(serverIP, portnum);
	if (result == TResult::FAIL)
		return TResult::FAIL;


	return result;
}

