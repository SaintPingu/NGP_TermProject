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
///	  Window Socket 초기화
/// -------------------------+	
	//WSADATA wsa{};
	//if((::WSAStartup(MAKEWORD(2,2), &wsa)) == 0)
	//	return false;
		

/// +-------------------------
///	  TCPListenNetwork 초기화
/// -------------------------+	

	ListenNet = new TCPListenNetwork();
	if (ListenNet->Init() == TResult::SUCCESS)
	{
		Res = ListenNet->CreateSocket();
		Res = ListenNet->BindListen(9000); // 임시 포트번호  
	}
	else
		return false;


/// +--------------------------------------------
///	   CLIENT MGR ( ServerNetwork 관리 ) 초기화
/// --------------------------------------------+	
	if (!CLIENT_MGR->Init())
		return false;

/// +--------------------------------------------
///	   SceneMgr 초기화
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

	// TODO : ServerFramework 삭제 전 수행 

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

