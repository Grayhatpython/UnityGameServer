#pragma once
#include "Protocol.pb.h"

using PacketProcessingFunction = std::function<bool(std::shared_ptr<PacketSession>&, BYTE*, int32)>;
extern PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

enum : uint16
{
//	Auto
	PACKET_C_LOGIN = 1,
	PACKET_S_LOGIN = 2,
	PACKET_C_ENTER_GAME = 3,
	PACKET_S_ENTER_GAME = 4,
	PACKET_C_LEAVE_GAME = 5,
	PACKET_S_LEAVE_GAME = 6,
	PACKET_S_SPAWN = 7,
	PACKET_S_DESPAWN = 8,
	PACKET_C_CHAT = 9,
	PACKET_S_CHAT = 10,

};

//	Custom Packet Processing Function
bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);
//	Auto
bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet);
bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet);
bool S_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& packet);
bool S_SPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SPAWN& packet);
bool S_DESPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& packet);
bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet);


class ServerPacketHandler
{
public:
	static void Initialize()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketPacketProcessingFunction[i] = Packet_Processing_Function_Undefined;

		//	Register Function
		//	Auto
		GPacketPacketProcessingFunction[PACKET_S_LOGIN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_LOGIN>(S_LOGIN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_ENTER_GAME] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_ENTER_GAME>(S_ENTER_GAME_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_LEAVE_GAME] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_LEAVE_GAME>(S_LEAVE_GAME_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_SPAWN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_SPAWN>(S_SPAWN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_DESPAWN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_DESPAWN>(S_DESPAWN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHAT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHAT>(S_CHAT_Packet_Processing_Function, session, buffer, len); };

	}

	static bool PacketProcessing(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketPacketProcessingFunction[packetHeader->id](session, buffer, len);
	}

public:
	//	Auto
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LOGIN& packet) { return TMakeSendBuffer(packet, PACKET_C_LOGIN); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER_GAME& packet) { return TMakeSendBuffer(packet, PACKET_C_ENTER_GAME); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LEAVE_GAME& packet) { return TMakeSendBuffer(packet, PACKET_C_LEAVE_GAME); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHAT& packet) { return TMakeSendBuffer(packet, PACKET_C_CHAT); }


private:
	template<typename PacketType, typename ProcessFunction>
	static bool TPacketProcessing(ProcessFunction function, std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketType packet;

		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
		const uint16 dataSize = packetHeader->size - sizeof(PacketHeader);

		if (packet.ParseFromArray(&packetHeader[1], dataSize) == false)
			return false;

		return function(session, packet);
	}

	template<typename T>
	static std::shared_ptr<SendBuffer> TMakeSendBuffer(T& packet, uint16 packetId)
	{
		const uint16 dataSize = static_cast<uint16>(packet.ByteSizeLong());
		const uint16 packetTotalSize = dataSize + sizeof(PacketHeader);

		auto sendBuffer = GSendBufferManager->Open(packetTotalSize);
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		packetHeader->id = packetId;
		packetHeader->size = packetTotalSize;

		ASSERT_CRASH(packet.SerializeToArray(&packetHeader[1], dataSize));

		sendBuffer->Close(packetTotalSize);
		return sendBuffer;
	}
};