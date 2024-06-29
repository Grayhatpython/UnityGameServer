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

	//	Room Enter ������ ���ο� �÷��̾�� ����
	{
		Protocol::S_ENTER_GAME enterGamePacket;
		enterGamePacket.set_successed(successd);

		//	��� ����� ���� ���� �ִ�.
		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->_info);

		//	enterGamePacket �Ҹ� �� playerInfo* ����
		//	������ ���� ���������� �Ϲ� ������ó�� �������� �������� �ʾƵ� ����Ʈ������ó�� �˾Ƽ� �Ҹ���
		enterGamePacket.set_allocated_player(playerInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
		if (auto session = player->_ownerSession)
			session->Send(sendBuffer);
	}

	//	���ο� �÷��̾��� ������ ���� �÷��̾�鿡�� ����
	{
		Protocol::S_SPAWN spawnPacket;

		auto playerInfo = spawnPacket.add_players();
		playerInfo->CopyFrom(*player->_info);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);

		//	�� �ڽ��� ����
		//	�� �ڽ��� �̹� S_ENTER_GAME Packet���� ������ ���۹���
		Broadcast(sendBuffer, objectId);
	}

	//	���ο� �÷��̾�� ������ ���ӵǾ��ִ� �÷��̾� ����� ����
	{
		Protocol::S_SPAWN spawnPacket;

		for (auto& inGamePlayerInfo : _players)
		{
			//	�� �ڽ��� ����
			//	�� �ڽ��� �̹� S_ENTER_GAME Packet���� ������ ���۹���
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

	//	�θ޸� ���� ����
	auto objectId = player->_info->objectid();
	if (_players.find(objectId) == _players.end())
		return;

	player->_room = weak_ptr<Room>();
	_players.erase(objectId);

	{
		//	������ ���� ���ο��� ����
		Protocol::S_LEAVE_GAME leaveGamePacket;
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePacket);

		//	��...
		if (auto session = player->_ownerSession)
			session->Send(sendBuffer);
	}

	{
		//	������ ������ id�� �������ִµ� �������� ����
		Protocol::S_DESPAWN despawnPacket;
		despawnPacket.add_objectids(objectId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPacket);

		//	�� �ڽ��� ����
		//	�� �ڽ��� �̹� S_LEAVE_GAME Packet���� ������ ���۹���
		//	������ �̹� players�� �����̳ʿ��� ������ �ؼ� �ǹ� ���� ��..
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

		//	�����̳� �ݺ��� �߿� Send�� �ϰ� �ִµ�
		//	� �̽��� ���� _players �����̳ʿ� ���� or ���� �̽��� ����ٸ�?
		if (session)
			session->Send(sendBuffer);
	}
}

void Room::Update()
{
	//	Packet handle Processing per frame
	Execute(true);
}


