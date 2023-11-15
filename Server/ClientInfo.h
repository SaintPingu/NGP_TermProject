#pragma once

// 2023-11-15-WED (���繮) - Ŭ���̾�Ʈ �Ŵ������� ������ Ŭ���̾�Ʈ Ŭ���� ���� -> ClientMgr ���� struct ���¿��ٰ� Ŭ���̾�Ʈ ���� �� ���� ���׵��� �þ�� ���� �̷��� Ŭ������ �����ϱ�� ������ 


/// +------------------
///		CLIENT INFO
/// ------------------+

#include "PacketStruct.h"

class ServerNetwork;
enum class ConnectFlag
{
	none,
	recv,
	send,
	END,
};

class ClientInfo
{
private:
	int					ID = -1;				// Ŭ���̾�Ʈ ���̵� ( ClientMgr ������ �ε������� �� ���̵��̴� ) 
	ServerNetwork*		serverNet{};			// Ŭ���̾�Ʈ ���� ��Ʈ��ũ
	bool				executeLogic{};			// Ŭ���̾�Ʈ ���� ���� ����
	bool				active = true;

	PacketBuffer		packetBuffer{};
	ConnectFlag			curConnectFlag{};		// ���� �����ؾ��� ���� Recv/Send

public:
	TResult Logic();

private:
	TResult RecvPacket();
	TResult SendPacket();

public:
	TResult SetServerNet(ServerNetwork* net) { if (!serverNet) { serverNet = net; return TResult::SUCCESS; } return TResult::ERROR_SYNC_ISSUE; }
	void	SetID(int id) { ID = id; }
	void	SetConnectFlag(ConnectFlag flag) { curConnectFlag = flag; }


	const ServerNetwork* GetServerNet() { return serverNet; }
	int					 GetID() { return ID; }

	void Execute() { executeLogic = true; }
	void Stop() { active = false; }
	void Run() { active = true; }
	void Exit() { executeLogic = false; }

public:
	bool IsConnected(); 
	TResult Clear(); 

public:
	ClientInfo();
	~ClientInfo();

};