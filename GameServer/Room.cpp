#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"

RoomRef GRoom = nullptr;

Room::Room()
{

}

Room::~Room()
{

}

void Room::Enter(PlayerRef player)
{
	bool successd = false;

	auto objectId = player->_info->objectid();
	if (_players.find(objectId) != _players.end())
		successd = false;
	else
	{
		_players.insert(make_pair(objectId, player));

		//player->_room.store(GetRoomRef());
		player->_room = GetRoomRef();

		player->_info->set_x(Utils::GetRandom(0.f, 50.f));
		player->_info->set_y(0);
		player->_info->set_z(Utils::GetRandom(0.f, 50.f));
		player->_info->set_yaw(Utils::GetRandom(0.f, 100.f));
		successd = true;
	}

	//	Room Enter 성공을 새로운 플레이어에게 전송
	{
		Protocol::S_ENTER_GAME enterGamePacket;
		enterGamePacket.set_successed(successd);

		//	사실 방식은 여러 개가 있다.
		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->_info);

		//	enterGamePacket 소멸 시 playerInfo* 삭제
		//	성능은 조금 포기하지만 일반 포인터처럼 수동으로 관리하지 않아도 스마트포인터처럼 알아서 소멸함
		enterGamePacket.set_allocated_player(playerInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
		if (auto session = player->_ownerSession)
			session->Send(sendBuffer);
	}

	//	새로운 플레이어의 접속을 기존 플레이어들에게 전송
	{
		Protocol::S_SPAWN spawnPacket;

		auto playerInfo = spawnPacket.add_players();
		playerInfo->CopyFrom(*player->_info);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);

		//	나 자신은 제외
		//	나 자신은 이미 S_ENTER_GAME Packet으로 정보를 전송받음
		Broadcast(sendBuffer, objectId);
	}

	//	새로운 플레이어에게 기존에 접속되어있던 플레이어 목록을 전송
	{
		Protocol::S_SPAWN spawnPacket;

		for (auto& inGamePlayerInfo : _players)
		{
			//	나 자신은 제외
			//	나 자신은 이미 S_ENTER_GAME Packet으로 정보를 전송받음
			auto inGamePlayer = inGamePlayerInfo.second;
			if (inGamePlayer->_info->objectid() == objectId)
				continue;

			auto addPlayer = spawnPacket.add_players();
			addPlayer->CopyFrom(*inGamePlayer->_info);
		}

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);
		if (auto session = player->_ownerSession)
			session->Send(sendBuffer);
	}
}

void Room::Leave(PlayerRef player)
{
	if (player == nullptr)
		return;

	//	인메모리 삭제 과정
	auto objectId = player->_info->objectid();
	if (_players.find(objectId) == _players.end())
		return;

	player->_room = weak_ptr<Room>();
	_players.erase(objectId);

	{
		//	나가는 유저 본인에게 전송
		Protocol::S_LEAVE_GAME leaveGamePacket;
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePacket);

		//	흠...
		if (auto session = player->_ownerSession)
			session->Send(sendBuffer);
	}

	{
		//	나가는 유저의 id를 접속해있는데 유저에게 전송
		Protocol::S_DESPAWN despawnPacket;
		despawnPacket.add_objectids(objectId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPacket);

		//	나 자신은 제외
		//	나 자신은 이미 S_LEAVE_GAME Packet으로 정보를 전송받음
		//	위에서 이미 players의 컨테이너에서 삭제를 해서 의미 없긴 함..
		Broadcast(sendBuffer, objectId);

		//if (auto session = player->_ownerSession.lock())
		//	session->Send(sendBuffer);
	}
}

void Room::Move(Protocol::C_MOVE movePacket)
{
	//	TODO : player ID check 
	const auto objectId = movePacket.playerinfo().objectid();
	if (_players.find(objectId) == _players.end())
		return;

	auto& player = _players[objectId];
	//	TODO : Position Check
	//	Update Position
	player->_info->CopyFrom(movePacket.playerinfo());

	{
		Protocol::S_MOVE moveSendPacket;
		auto playerInfo = moveSendPacket.mutable_playerinfo();
		playerInfo->CopyFrom(movePacket.playerinfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(moveSendPacket);
		Broadcast(sendBuffer);
	}
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 ignoreId)
{
	for (auto& player : _players)
	{
		auto objectId = player.second->_info->objectid();

		if (objectId == ignoreId)
			continue;

		auto session = player.second->_ownerSession;

		//	컨테이너 반복문 중에 Send를 하고 있는데
		//	어떤 이슈로 인해 _players 컨테이너에 삭제 or 삽입 이슈가 생긴다면?
		if (session)
			session->Send(sendBuffer);
	}
}

void Room::Update()
{
	//	Packet handle Processing per frame
	Execute(true);
}


