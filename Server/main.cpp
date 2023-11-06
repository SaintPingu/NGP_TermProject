
#include "stdafx.h"
#include "ServerFramework.h"
#include <Thread>

/// +-- ���� ���� --+
// 2023-11-05-SUN (���繮) : ���� ������ ��ũ�� ū Ʋ�� �뷫������ ��ҽ��ϴ�. ���� ������ ��ũ ���� �� ���� ä�� ���Դϴ�.. 
// 2023-11-06-MON (���繮) : �����͸� �����ϰ� ��Ŷ ����ü�� ä�����ϴ�. ���� Cmd Enum class �� ä�� ���̸� �ٵ��� �����Դϴ�.. 


int main()
{

	/// +---------------------
	///	 SERVER FRAMEWORK RUN
	/// ---------------------+	
	if (SERVER_FRAMEWORK->Init())
	{
		std::thread Server([&]() {
			SERVER_FRAMEWORK->Execute();  }
		);

		SERVER_FRAMEWORK->Exit();
		SERVER_FRAMEWORK->Destroy();
		Server.join();
	}
	
}