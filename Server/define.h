#pragma once
#define SINGLETON_PATTERN(TYPE)				\
private:									\
   static TYPE* mInst;						\
public:										\
   static TYPE* Inst()						\
   {										\
      if (!mInst) mInst = new TYPE;			\
      return mInst;							\
   }										\
   static void Destroy() {					\
      if (nullptr != mInst) {				\
         delete mInst;						\
         mInst = nullptr;					\
      }										\
   }                  

#define SINGLETON_PATTERN_DEFINITION(TYPE)  \
   TYPE* TYPE::mInst = nullptr;

#define SERVER_FRAMEWORK ServerFramework::Inst()



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