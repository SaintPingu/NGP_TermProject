#pragma once

// 2023-11-05 : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.

class ServerFramework
{
	SINGLETON_PATTERN(ServerFramework);
	
private:
	bool ExecuteFramework = true; // ���� ����


public:
	bool Init();
	void Execute();
	void Exit();
	void Stop();

public:
	ServerFramework();
	~ServerFramework();


};

