
#include "stdafx.h"
#include "ServerFramework.h"

/// +-- ���� ���� --+
// 2023-11-05-SUN (���繮) : ���� ������ ��ũ�� ū Ʋ�� �뷫������ ��ҽ��ϴ�. ���� ������ ��ũ ���� �� ���� ä�� ���Դϴ�.. 
// 2023-11-06-MON (���繮) : �����͸� �����ϰ� ��Ŷ ����ü�� ä�����ϴ�. ���� Cmd Enum class �� ä�� ���̸� �ٵ��� �����Դϴ�.. 
// 2023-11-08-WED (���繮) : ��Ŷ ���� ������ ������Ʈ, ���� CMD enum ����, �����ӿ�ũ Main Logic, ListenNetwork ���� ū Ʋ ���, ���� Log Ŭ�����z ���� ���� �α� ������ ���� 
// 2023-11-08-WED (������) : DataBase �߰� 
// 2023-11-10-FRI (���繮) : ListenNetwork ���� ��,, ClientMgr ���� ��.. ** Ŭ���̾�Ʈ ������Ʈ ���� ���� ���� -> x86 �̾���.. **



int main()
{


	/// +---------------------
	///	 SERVER FRAMEWORK RUN
	/// ---------------------+	
	if (SERVER_FRAMEWORK->Init())
	{
		SERVER_FRAMEWORK->Execute(); 
		SERVER_FRAMEWORK->Exit();
	}

}