#pragma once



#define SERVER_FRAMEWORK ServerFramework::Inst()
#define CLIENT_MGR ClientMgr::Inst()
#define SCENE_MGR SERVER_FRAMEWORK->GetSceneMgr()

#define DATABASE DataBase::Inst()

#define MAX_IP_LEN 32

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

/*---------------
	  Crash
---------------*/

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}