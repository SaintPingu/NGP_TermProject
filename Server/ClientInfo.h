#pragma once

// 2023-11-15-WED (장재문) - 클라이언트 매니저에서 관리할 클라이언트 클래스 생성 -> ClientMgr 에서 struct 형태였다가 클라이언트 정보 및 관리 사항들이 늘어남에 따라 이렇게 클래스로 관리하기로 결정함 


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
	int					ID = -1;				// 클라이언트 아이디 ( ClientMgr 에서의 인덱스값이 곧 아이디이다 ) 
	ServerNetwork*		serverNet{};			// 클라이언트 서버 네트워크
	bool				executeLogic{};			// 클라이언트 로직 구동 여부
	bool				active = true;

	CommandList			cmdList{};
	PacketBuffer		packetBuffer{};
	ConnectFlag			curConnectFlag{};		// 현재 연결해야할 상태 Recv/Send

	HANDLE				sendEvent{};			// 패킷 전송 이벤트

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