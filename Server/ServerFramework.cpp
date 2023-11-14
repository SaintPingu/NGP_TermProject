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
	: executeFramework(false)
	, listenNet(nullptr)
{

}

ServerFramework::~ServerFramework()
{
	SAFE_DELETE(listenNet);
	CLIENT_MGR->Destroy();
}

bool ServerFramework::Init()
{
	bool Result = true;
	TResult Res{};

/// +-------------------------
///	  Window Socket 초기화
/// -------------------------+	
	WSADATA wsa{};
	if((::WSAStartup(MAKEWORD(2,2), &wsa)) != 0)
		return false;
		

/// +-------------------------
///	  TCPListenNetwork 초기화
/// -------------------------+	
	listenNet = new TCPListenNetwork();
	if (listenNet->Init() != TResult::SUCCESS)
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
	executeFramework = true;
	return Result;
}



void ServerFramework::Execute()
{
	std::cout << "-> Server Execute\n";

	/// +-------------------------
	///	    Thread For Listen.
	/// -------------------------+	
	std::thread Listen([&]() {
		listenNet->Logic();
		});

	/// +-------------------------
	///	    Thread For Server.
	/// -------------------------+			
	ServerFramework::Logic();


	Listen.join();
	std::cout << "-> Server Exit\n";

}



void ServerFramework::Exit()
{
	SERVER_FRAMEWORK->Stop();

	// TODO : ServerFramework 삭제 전 수행 

}

void ServerFramework::Stop()
{
	executeFramework = false;
	listenNet->Stop();

}

void ServerFramework::Logic()
{

	std::cout << "\t-> Server Logic 구동 중 ...\n";
	while (executeFramework)
	{
		Timer::Inst()->Tick(30.f);
		SetPacketBuffer();
		ProcessCommand();
		UpdateScene();
		SendPakcet();

		// 아래 코드를 삭제하면 서버 프레임 워크가 동작한다.  
		SERVER_FRAMEWORK->Stop();

	}

	std::cout << "\t-> Server Logic Exit\n";

}

TResult ServerFramework::Start()
{
	Timer::Inst()->Reset();


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

