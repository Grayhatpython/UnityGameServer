#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"
#include "Room.h"
#include "Player.h"

ClientSession::~ClientSession()
{
	cout << "~ClientSession" << endl;
}

void  ClientSession::OnConnected() 
{
	GClientSessionManager->Add(static_pointer_cast<ClientSession>(shared_from_this()));
}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSessionRef();
	//	PacketId üũ
	ClientPacketHandler::PacketProcessing(session, buffer, len);
}

void  ClientSession::OnSend(int32 len) 
{
	//cout << "OnSend Len = " << len << endl;
}

void  ClientSession::OnDisconnected() 
{
	GClientSessionManager->Remove(static_pointer_cast<ClientSession>(shared_from_this()));

	if (_player)
	{
		if (auto room = _player->_room.lock())
			room->PushJob(&Room::Leave, true, _player->_objectInfo->objectid());
	}

	_player = nullptr;
}