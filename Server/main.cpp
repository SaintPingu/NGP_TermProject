
#include "stdafx.h"
#include "ServerFramework.h"

/// +-- 개발 일지 --+
// 2023-11-05-SUN (장재문) : 서버 프레임 워크의 큰 틀을 대략적으로 잡았습니다. 향후 프레임 워크 보완 및 살을 채울 것입니다.. 
// 2023-11-06-MON (장재문) : 데이터를 생성하고 패킷 구조체를 채웠습니다. 향후 Cmd Enum class 를 채울 것이며 다듬을 예정입니다.. 
// 2023-11-08-WED (장재문) : 패킷 내부 데이터 업데이트, 서버 CMD enum 생성, 프레임워크 Main Logic, ListenNetwork 구동 큰 틀 잡기, 향후 Log 클래스틑 통해 서버 로그 구동할 것임 
// 2023-11-08-WED (최정일) : DataBase 추가 
// 2023-11-10-FRI (장재문) : ListenNetwork 구현 중,, ClientMgr 구현 중.. ** 클라이언트 프로젝트 에러 나고 있음 -> x86 이었음.. **



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