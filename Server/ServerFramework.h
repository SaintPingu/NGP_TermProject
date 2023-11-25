#pragma once

// 2023-11-05-SUN (���繮) : ū Ʋ�� ��� ���ؼ� �̸� �����Ǿ����ϴ�.
// 2023-11-08-WED (���繮) : ���� �����ӿ�ũ ���� ����  
// 2023-11-13-MON (�ε���) : SceneMgr ���� 

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
	bool						executeFramework{};	// ����
	TCPListenNetwork*			listenNet{};
	std::shared_ptr<SceneMgr>	sceneMgr{};

public:
	bool Init();		// �ʱ�ȭ
	void Execute();		// ����
	void Exit();		// ������
	void Stop();		// ����


	/// +--------------------------
	///	 SERVER FRAMEWORK - LOGIC 
	/// --------------------------+	
private:
	void Logic();

	TResult Start();			// 0. ����
	TResult Event();			// 1. �̺�Ʈ ó��
	TResult SetPacketBuffer();  // 2. ��Ŷ ����		- �� Ŭ���̾�Ʈ�� ��Ŷ�� �����Ѵ�. 
	TResult ProcessCommand();   // 3. ��Ŷ �ؼ�		- Ŭ���̾�Ʈ�κ��� ���� ��Ŷ�� �ؼ��ϰ� ������ ������Ʈ 
	TResult UpdateScene();		// 4. �� ����		- �� ������Ʈ 
	TResult SendPakcet();		// 5. ��Ŷ �۽�		- ��Ŷ�� �� Ŭ���̾�Ʈ���� �۽�  


public:
	ServerFramework();
	~ServerFramework();

	const std::shared_ptr<SceneMgr>& GetSceneMgr() const { return sceneMgr; }
};

