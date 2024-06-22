#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "ServerSession.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	return true;
}

/*
//	사실 session은 현재 서버임
bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet)
{
	if (packet.success() == false)
		return false;

	if (packet.players().size() == 0)
	{
		//	캐릭터 생성
	}

	//	입장 UI 버튼 -> 게임 입장 
	Protocol::C_ENTER_GAME enterGamePacket;
	enterGamePacket.set_playerindex(0);	//	현재 무조건 첫번째 캐릭터 입장
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePacket);
	session->Send(sendBuffer);

	return true;
}

bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet)
{
	if (packet.success() == false)
		return false;


	return true;
}

bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet)
{
	cout << packet.msg() << endl;
	return true;
}
*/

bool S_BROADCASTENTERGAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_BROADCASTENTERGAME& packet)
{
	return true;
}

bool S_BROADCASTLEAVEGAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_BROADCASTLEAVEGAME& packet)
{
	return true;
}

bool S_PLAYERLIST_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_PLAYERLIST& packet)
{
	return true;
}

bool S_BROADCASTMOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_BROADCASTMOVE& packet)
{
	return true;
}
