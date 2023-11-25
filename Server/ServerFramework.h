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
	TResult SetPacketBuffer();  // 2. 패킷 세팅		- 각 클라이언트의 패킷을 세팅한다. 
	TResult ProcessCommand();   // 3. 패킷 해석		- 클라이언트로부터 받은 패킷을 해석하고 데이터 업데이트 
	TResult UpdateScene();		// 4. 씬 갱신		- 씬 업데이트 
	TResult SendPakcet();		// 5. 패킷 송신		- 패킷을 각 클라이언트에게 송신  


public:
	ServerFramework();
	~ServerFramework();

	const std::shared_ptr<SceneMgr>& GetSceneMgr() const { return sceneMgr; }
};

