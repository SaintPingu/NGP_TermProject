#pragma once

// 2023-11-15-WED (���繮) - Ŭ���̾�Ʈ �Ŵ������� ������ Ŭ���̾�Ʈ Ŭ���� ���� -> ClientMgr ���� struct ���¿��ٰ� Ŭ���̾�Ʈ ���� �� ���� ���׵��� �þ�� ���� �̷��� Ŭ������ �����ϱ�� ������ 


/// +------------------
///		CLIENT INFO
/// ------------------+

#include "PacketStruct.h"

class ServerNetwork;
class ClientInfo
{
private:
	std::thread thread{};

private:
	int					ID = -1;				// Ŭ���̾�Ʈ ���̵� ( ClientMgr ������ �ε������� �� ���̵��̴� ) 
	ServerNetwork*		serverNet{};			// Ŭ���̾�Ʈ ���� ��Ʈ��ũ
	bool				executeLogic{};			// Ŭ���̾�Ʈ ���� ���� ����
	bool				active = true;

	CommandList			cmdList{};
	PacketBuffer		packetBuffer{};
	ConnectFlag			curConnectFlag{};		// ���� �����ؾ��� ���� Recv/Send

	HANDLE				sendEvent{};			// ��Ŷ ���� �̺�Ʈ

public:
	void CreateThread();
	TResult Logic();

private:
	TResult RecvPacket();
	TResult SendPacket();

public:
	TResult SetServerNet(ServerNetwork* net) { if (!serverNet) { serverNet = net; return TResult::SUCCESS; } return TResult::ERROR_SYNC_ISSUE; }
	void	SetID(int id);
	void	SetConnectFlag(ConnectFlag flag) { curConnectFlag = flag; }
	void	Send() { SetEvent(sendEvent); }


	ConnectFlag GetConnectFlag() const { return curConnectFlag; }
	const ServerNetwork* GetServerNet() { return serverNet; }
	PacketBuffer& GetPacketBuffer();
	int					 GetID() { return ID; }
	CommandList* GetCmdList() { return &cmdList; }

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