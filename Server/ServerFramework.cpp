#include "stdafx.h"
#include "ServerFramework.h"

#include "TCPNetwork.h"
#include "TCPListenNetwork.h"
#include "PacketNetwork.h"
#include "ServerNetwork.h"

#include "ClientMgr.h"
#include "SceneMgr.h"


SINGLETON_PATTERN_DEFINITION(ServerFramework);

ServerFramework::ServerFramework()
	: ExecuteFramework(false)
	, ListenNet(nullptr)
{

}

ServerFramework::~ServerFramework()
{
}

bool ServerFramework::Init()
{
	bool Result = true;
	TResult Res{};

/// +-------------------------
///	  Window Socket �ʱ�ȭ
/// -------------------------+	
	//WSADATA wsa{};
	//if((::WSAStartup(MAKEWORD(2,2), &wsa)) == 0)
	//	return false;
		

/// +-------------------------
///	  TCPListenNetwork �ʱ�ȭ
/// -------------------------+	

	ListenNet = new TCPListenNetwork();
	if (ListenNet->Init() == TResult::SUCCESS)
	{
		Res = ListenNet->CreateSocket();
		Res = ListenNet->BindListen(9000); // �ӽ� ��Ʈ��ȣ  
	}
	else
		return false;


/// +--------------------------------------------
///	   CLIENT MGR ( ServerNetwork ���� ) �ʱ�ȭ
/// --------------------------------------------+	
	if (!CLIENT_MGR->Init())
		return false;

/// +--------------------------------------------
///	   SceneMgr �ʱ�ȭ
/// --------------------------------------------+	
	sceneMgr = std::make_shared<SceneMgr>();
	sceneMgr->Init();

/// +-------------------------
///	  Server Framework Start
/// -------------------------+	
	ServerFramework::Start();
	ExecuteFramework = true;
	return Result;
}



void ServerFramework::Execute()
{
	while (ExecuteFramework)
	{
		std::cout << "-> Hello Server" << std::endl;
		
		/// +-------------------------
		///	    Thread For Listen.
		/// -------------------------+	
		{
			std::thread Listen([&]() {
				ListenNet->Logic(); 
				});
			Listen.join();
		}
		
		/// +-------------------------
		///	    Thread For Server.
		/// -------------------------+			
		{
			ServerFramework::Logic();
		}

		// TEST 
		SERVER_FRAMEWORK->Stop();
	}
}



void ServerFramework::Exit()
{
	SERVER_FRAMEWORK->Stop();

	// TODO : ServerFramework ���� �� ���� 

}

void ServerFramework::Stop()
{
	ExecuteFramework = false;
}

void ServerFramework::Logic()
{

	SetPacketBuffer();
	ProcessCommand();
	UpdateScene();
	SendPakcet();

}

TResult ServerFramework::Start()
{
	


	return TResult();

}

TResult ServerFramework::SetPacketBuffer()
{

	return TResult();
}

TResult ServerFramework::ProcessCommand()
{

	return TResult();
}

TResult ServerFramework::UpdateScene()
{

	return TResult();
}

TResult ServerFramework::SendPakcet()
{

	return TResult();
}

