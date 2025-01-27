#include "stdafx.h"
#include "PacketNetwork.h"

constexpr uint32 terminateCode = UINT8_MAX;
TResult PacketNetwork::Init()
{
    return TResult();
}

void PushDataLen(PacketBuffer& buffer)
{
    uint32 dataSize = buffer.size();
    buffer.resize(buffer.size() + size_uint32);
    memcpy(buffer.data() + size_uint32, buffer.data(), dataSize);

    for (int i = 0; i < size_uint32; ++i) {
        BYTE byte = (dataSize >> (8 * i)) & 0xFF;
        buffer[i] = byte;
    }
}

TResult PacketNetwork::SendPacket()
{
    if (PacketBuf.empty()) {
        std::cout << "[ERROR - SendPacket()] 송신할 버퍼가 비어있습니다!!\n";
        return TResult::SEND_SIZE_ZERO;
    }

    PushDataLen(PacketBuf);

    //std::cout << "Packet 송신 대기 : 포트 번호 [" << ntohs(TCP_SockAddr.sin_port) << "]\n";
    int retval = send(TCP_Socket, (const char*)PacketBuf.data(), PacketBuf.size(), 0);
    if (retval == SOCKET_ERROR) {
        err_display("Send Error");
        return TResult::FAIL;
    }

    //std::cout << "Packet 송신 완료 : 포트 번호 [" << ntohs(TCP_SockAddr.sin_port) << "] 패킷 길이[" << PacketBuf.size() << "]\n";
    PacketBuf.clear();


    return TResult::NONE;
}

TResult PacketNetwork::RecvPacket()
{
    //std::cout << "Packet 수신 대기 : 포트 번호 [" << ntohs(TCP_SockAddr.sin_port) << "]\n";
    int retval{};
    uint32 dataLen;
    retval = recv(TCP_Socket, (char*)&dataLen, size_uint32, MSG_WAITALL);
    if (retval == SOCKET_ERROR) {
        return TResult::FAIL;
    }

    //std::cout << "Packet 헤더 수신 완료 : 데이터 길이[" << dataLen << "]\n";

    if (dataLen == terminateCode) {
        return TResult::CLIENT_DISCONNECT;
    }

    if (dataLen > 0) {
        PacketBuf.resize(dataLen);
        retval = recv(TCP_Socket, (char*)PacketBuf.data(), dataLen, MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("Recv Error");
            return TResult::FAIL;
        }
    }


    //std::cout << "Packet 수신 완료 : 포트 번호 [" << ntohs(TCP_SockAddr.sin_port) << "] 데이터 길이[" << PacketBuf.size() << "]\n";

    return TResult::NONE;
}

TResult PacketNetwork::SendTerminatePacket()
{
    PacketBuf.clear();
    PacketBuf.insert(PacketBuf.begin(), &terminateCode, &terminateCode + size_uint32);
    return SendPacket();
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
