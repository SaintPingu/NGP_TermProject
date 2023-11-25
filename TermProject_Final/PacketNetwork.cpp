#include "stdafx.h"
#include "PacketNetwork.h"

TResult PacketNetwork::Init()
{
    return TResult();
}

TResult PacketNetwork::SendPacket()
{
    int retval = send(TCP_Socket, (const char*)PacketBuf.data(), PacketBuf.size(), 0);
    if (retval == SOCKET_ERROR) {
        err_display("Send Error");
        return TResult::FAIL;
    }
    //std::cout << "송신한 패킷 길이 : " << PacketBuf.size() << std::endl;

    PacketBuf.clear();

    return TResult::NONE;
}

TResult PacketNetwork::RecvPacket()
{
    //std::cout << "데이터 길이 수신 대기\n";

    int retval{};
    int dataLen{};
    retval = recv(TCP_Socket, (char*)&dataLen, sizeof(int), MSG_WAITALL);
    if (retval == SOCKET_ERROR) {
        err_display("Recv Error");
        return TResult::FAIL;
    }

    //std::cout << "수신한 데이터 길이 : " << (int)dataLen << std::endl;
    if (dataLen > 0) {
        PacketBuf.resize(dataLen);
        retval = recv(TCP_Socket, (char*)PacketBuf.data(), dataLen, MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("Recv Error");
            return TResult::FAIL;
        }
    }

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
