
#include "stdafx.h"
#include "ServerFramework.h"
#include <Thread>

// 2023-11-05-SUN (장재문) : 서버 프레임 워크의 큰 틀을 대략적으로 잡았습니다. 향후 프레임 워크 보완 및 살을 채울 것입니다.. 

int main()
{

	/// +---------------------
	///	 SERVER FRAMEWORK RUN
	/// ---------------------+	
	/// 


	if (SERVER_FRAMEWORK->Init())
	{
		std::thread FrameworkThread([&]() {
			SERVER_FRAMEWORK->Execute();  }
		);

		SERVER_FRAMEWORK->Stop();
		SERVER_FRAMEWORK->Exit();
		SERVER_FRAMEWORK->Destroy();
		FrameworkThread.join();
	}
	
}