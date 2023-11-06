
#include "stdafx.h"
#include "ServerFramework.h"
#include <Thread>

// 2023-11-05-SUN (���繮) : ���� ������ ��ũ�� ū Ʋ�� �뷫������ ��ҽ��ϴ�. ���� ������ ��ũ ���� �� ���� ä�� ���Դϴ�.. 

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