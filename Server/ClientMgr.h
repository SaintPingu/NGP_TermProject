#pragma once

// 2023-11-06 (장재문) : 틀을 잡기 위해 미리 생성함. 네트워크를 통해 들어온 클라이언트들을 관리하는 매니저. - 향후 동기화에 주의하자. 

class ServerNetwork;
class ClientMgr
{
	SINGLETON_PATTERN(ClientMgr);
private:
	std::vector<ServerNetwork*> ClientPool;
//  PacketGenerator				PacketGen;		// ( Packet Encoder )
	Mutex						Mute;

public:
	void CreateClientThread();
	bool SetPacketBuffer();
	bool SendPacket();
	bool CheckInsertedSocket(); 
	void PushCommand();

public:
	ClientMgr() {};
	~ClientMgr() {};

};

