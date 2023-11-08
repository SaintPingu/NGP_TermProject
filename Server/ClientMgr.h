#pragma once

// 2023-11-06 (���繮) : Ʋ�� ��� ���� �̸� ������. ��Ʈ��ũ�� ���� ���� Ŭ���̾�Ʈ���� �����ϴ� �Ŵ���. - ���� ����ȭ�� ��������. 

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

