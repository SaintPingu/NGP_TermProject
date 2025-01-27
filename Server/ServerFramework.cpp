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

static constexpr int FPS = 60;

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

	std::cout << "\t-> Server Logic 구동 중 ... [P : 종료]\n";
	while (executeFramework)
	{
		static int cnt{};
		std::cout << "\t\t\t\t-> server logic [" << ++cnt << "]\r";
		if (cnt >= FPS) {
			cnt = 0;
		}

		bool isSendPacket{ false };
		if (cnt & 1) {	// (60FPS에서 30프레임마다 데이터 송신)
			isSendPacket = true;
		}
		Event();
		Update(isSendPacket);
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
	CLIENT_MGR->Event();
	SCENE_MGR->Event();

	return TResult();
}

TResult ServerFramework::Update(bool isSendPacket)
{
	Timer::Inst()->Tick(FPS);

	if (isSendPacket) {
		CLIENT_MGR->SetPacketBuffer();
		CLIENT_MGR->ProcessCommand();
		UpdateScene();
		SendPakcet();
	}
	else {
		UpdateScene();
	}

	//if (GetAsyncKeyState('~') & 0x8000)
	//	SERVER_FRAMEWORK->Exit();


	return TResult();
}


TResult ServerFramework::UpdateScene()
{
	SCENE_MGR->UpdateScenes();
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

