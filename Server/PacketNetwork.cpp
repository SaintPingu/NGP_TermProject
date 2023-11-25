#include "stdafx.h"
#include "PacketNetwork.h"

TResult PacketNetwork::Init()
{
    return TResult();
}

TResult PacketNetwork::SendPacket()
{
    //std::cout << "�۽� ���\n";
    int retval = send(TCP_Socket, (const char*)PacketBuf.data(), PacketBuf.size(), 0);
    if (retval == SOCKET_ERROR) {
        err_display("Send Error");
        return TResult::FAIL;
    }

    //std::cout << "�۽� �Ϸ� : ��Ʈ ��ȣ [" << ntohs(TCP_SockAddr.sin_port) << "] ��Ŷ ����[" << PacketBuf.size() << "]\n";
    PacketBuf.clear();


    return TResult::NONE;
}

TResult PacketNetwork::RecvPacket()
{
    //std::cout << "���� ���\n";
    int retval{};
    char dataLen;
    retval = recv(TCP_Socket, &dataLen, sizeof(char), MSG_WAITALL);
    if (retval == SOCKET_ERROR) {
        return TResult::FAIL;
    }

    if (dataLen > 0) {
        PacketBuf.resize(dataLen);
        retval = recv(TCP_Socket, (char*)PacketBuf.data(), dataLen, MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("Recv Error");
            return TResult::FAIL;
        }
    }


    //std::cout << "���� �Ϸ� : ��Ʈ ��ȣ [" << ntohs(TCP_SockAddr.sin_port) << "] ������ ����[" << PacketBuf.size() << "]\n";

    return TResult::NONE;
}

void PacketNetwork::SetPacketBuffer(const Packet& packet)
{
    PacketBuf.resize(packet.size());
    ::memcpy(PacketBuf.data(), packet.data(), packet.size());
}

PacketNetwork::PacketNetwork()
{
}

PacketNetwork::~PacketNetwork()
{
}
