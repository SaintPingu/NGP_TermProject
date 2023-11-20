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
        return TResult::FAIL;
    }
    PacketBuf.clear();

    return TResult::NONE;
}

TResult PacketNetwork::RecvPacket()
{
    int retval{};
    char dataLen;
    retval = recv(TCP_Socket, &dataLen, sizeof(char), MSG_WAITALL);
    if (retval == SOCKET_ERROR) {
        return TResult::FAIL;
    }

    PacketBuf.resize(dataLen);
    retval = recv(TCP_Socket, (char*)PacketBuf.data(), dataLen, MSG_WAITALL);
    if (retval == SOCKET_ERROR) {
        return TResult::FAIL;
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
