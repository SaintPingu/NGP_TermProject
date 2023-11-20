
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
	// 콘솔창 띄우기 
	AllocConsole();
	// 표준 입력, 출력 및 오류를 콘솔로 설정합니다.
	FILE* consoleIn, * consoleOut, * consoleErr;
	freopen_s(&consoleIn, "CONIN$", "r", stdin);
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
	freopen_s(&consoleErr, "CONOUT$", "w", stderr);

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



