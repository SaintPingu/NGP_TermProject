#include "stdafx.h"
#include "ServerFramework.h"

#include "TCPNetwork.h"
#include "TCPListenNetwork.h"
#include "PacketNetwork.h"
#include "ServerNetwork.h"


SINGLETON_PATTERN_DEFINITION(ServerFramework);

ServerFramework::ServerFramework()
{
}

ServerFramework::~ServerFramework()
{
}

bool ServerFramework::Init()
{
	bool Result = true;
	// TODO : ���� ������ ��ũ �ʱ�ȭ 

	return Result;
}

void ServerFramework::Execute()
{
	// TODO : ���� ������ ��ũ ���� ���� 
	while (ExecuteFramework)
	{
		std::cout << "Hello Server" << std::endl;

		// 1. Listen Network ----> Client Connect

		// 2. ServerNetwork  ----> Client Thread Run  
		{

		}
		SERVER_FRAMEWORK->Stop();
	}
}

void ServerFramework::Exit()
{
	// TODO : ���� ������ ��ũ ���� 
	SERVER_FRAMEWORK->Stop();

}

void ServerFramework::Stop()
{
	ExecuteFramework = false;
}

