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
///	  Window Socket �ʱ�ȭ
/// -------------------------+	
	WSADATA wsa{};
	if((::WSAStartup(MAKEWORD(2,2), &wsa)) != 0)
		return false;
		

/// +-------------------------
///	  TCPListenNetwork �ʱ�ȭ
/// -------------------------+	
	listenNet = new TCPListenNetwork();
	if (listenNet->Init() != TResult::SUCCESS)
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

	// TODO : ServerFramework ���� �� ���� 

}

void ServerFramework::Stop()
{
	executeFramework = false;
	listenNet->Stop();

}

void ServerFramework::Logic()
{

	std::cout << "\t-> Server Logic ���� �� ...\n";
	while (executeFramework)
	{
		Timer::Inst()->Tick(30.f);
		SetPacketBuffer();
		ProcessCommand();
		UpdateScene();
		SendPakcet();

		// �Ʒ� �ڵ带 �����ϸ� ���� ������ ��ũ�� �����Ѵ�.  
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

