#pragma once

// 2023-11-05-SUN (장재문) : 큰 틀을 잡기 위해서 미리 생성되었습니다.
// 2023-11-08-WED (장재문) : 서버 프레임워크 로직 생성  
// 2023-11-13-MON (민동현) : SceneMgr 구현 

/// +------------------
///	 SERVER FRAMEWORK
/// ------------------+

class TCPListenNetwork;
class ServerNetwork;
class SceneMgr;

class ServerFramework
{
	SINGLETON_PATTERN(ServerFramework);
	
private:
	bool						executeFramework{};	// 구동
	TCPListenNetwork*			listenNet{};
	std::shared_ptr<SceneMgr>	sceneMgr{};

public:
	bool Init();		// 초기화
	void Execute();		// 실행
	void Exit();		// 나가기
	void Stop();		// 중지

	/// +--------------------------
	///	 SERVER FRAMEWORK - LOGIC 
	/// --------------------------+	
private:
	void Logic();

	TResult Start();			// 0. 시작
	TResult Event();			// 1. 이벤트 처리
	TResult Update();			// 2. 업데이트 
	TResult SendPakcet();		// 3. 패킷 송신		- 패킷을 각 클라이언트에게 송신  

private:
	TResult UpdateScene();		// 씬 갱신		- 씬 업데이트 


public:
	ServerFramework();
	~ServerFramework();

	const std::shared_ptr<SceneMgr>& GetSceneMgr() const { return sceneMgr; }
};

