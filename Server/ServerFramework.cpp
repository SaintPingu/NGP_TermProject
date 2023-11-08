#include "stdafx.h"
#include "ServerFramework.h"

#include "TCPNetwork.h"
#include "TCPListenNetwork.h"
#include "PacketNetwork.h"
#include "ServerNetwork.h"


SINGLETON_PATTERN_DEFINITION(ServerFramework);

ServerFramework::ServerFramework()
	: ExecuteFramework(false)
	, ListenNet(nullptr)
	, ServerNet(nullptr)
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
///	  TCPListenNetwork �ʱ�ȭ
/// -------------------------+	

	ListenNet = new TCPListenNetwork();
	if (ListenNet->Init())
	{
		Res = ListenNet->CreateSocket();
		Res = ListenNet->BindListen(9000); // �ӽ� ��Ʈ��ȣ  
	}
	else
		return false;


/// +-------------------------
///	   ServerNetwork �ʱ�ȭ
/// -------------------------+	
	ServerNet = new ServerNetwork();
	if (ServerNet->Init())
	{

	}
	else
		return false;

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
		///	  Thread For Each Client.
		/// -------------------------+			
		{
			// ClientMgr ���� ���� 

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

