#include "stdafx.h"
#include "ServerFramework.h"

#include "TCPNetwork.h"
#include "TCPListenNetwork.h"
#include "PacketNetwork.h"
#include "ServerNetwork.h"

#include "ClientMgr.h"
#include "SceneMgr.h"
#include "ServerPacket.h"
#include "DataBase.h"


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
	SAFE_DELETE(packetGenerator);
	SAFE_DELETE(packetLoader);
}

bool ServerFramework::Init()
{
	bool Result = true;
	TResult Res{};

/// +-------------------------
///	  Window Socket 초기화
/// -------------------------+	
	WSADATA wsa{};
	if((::WSAStartup(MAKEWORD(2,2), &wsa)) != 0) // Exit() 에서 WSACleanup
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


/// +--------------------------------------------
///	   DATABASE 초기화
/// --------------------------------------------+	
	DATABASE->Init();


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

	/// +----------------------------------------
	///	    Thread For Server. ( main Thread )
	/// ----------------------------------------+			
	ServerFramework::Logic();


	Listen.join();
	std::cout << "-> Server Exit\n";

}

void ServerFramework::Logic()
{

	std::cout << "\t-> Server Logic 구동 중 ... [ENTER : 종료]\n";
	int cnt{};
	while (executeFramework)
	{
		Timer::Inst()->Tick(30.f);

		//std::cout << "\t\t\t-> server logic [" << ++cnt << "]\r";
		if (cnt >= 30) {
			cnt = 0;
		}

		
		Event();
		SetPacketBuffer();
		ProcessCommand();
		UpdateScene();
		SendPakcet();

		if(GetAsyncKeyState(VK_RETURN) & 0x8000)
			SERVER_FRAMEWORK->Exit();

	}

	std::cout << "\t-> Server Logic Exit\n";

}

TResult ServerFramework::Start()
{
	Timer::Inst()->Reset();


	return TResult();

}

TResult ServerFramework::Event()
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
	if (CLIENT_MGR->SendPacket()) {
		return TResult::NONE;
	}

	return TResult::FAIL;
}



void ServerFramework::Exit()
{
	if (!executeFramework)
		return;

	// TODO : ServerFramework 삭제 전 수행 
	executeFramework = false;

	listenNet->Exit();
	CLIENT_MGR->Exit();

	// 윈속 종료
	WSACleanup();
}

void ServerFramework::Stop()
{
	listenNet->Stop();
	CLIENT_MGR->Stop();

}

