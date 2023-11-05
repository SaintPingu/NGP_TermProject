
#include "stdafx.h"
#include "ServerFramework.h"
#include <Thread>

int main()
{

	if (SERVER_FRAMEWORK->Init())
	{
		std::thread FrameworkThread([&]() {
			SERVER_FRAMEWORK->Execute();  }
		);

		SERVER_FRAMEWORK->Exit();
		SERVER_FRAMEWORK->Destroy();
		FrameworkThread.join();
	}
	

}