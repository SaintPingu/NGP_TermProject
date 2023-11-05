#pragma once

// 2023-11-05 : 큰 틀을 잡기 위해서 미리 생성되었습니다.

class ServerFramework
{
	SINGLETON_PATTERN(ServerFramework);
	
private:
	bool ExecuteFramework = true; // 구동 변수


public:
	bool Init();
	void Execute();
	void Exit();
	void Stop();

public:
	ServerFramework();
	~ServerFramework();


};

