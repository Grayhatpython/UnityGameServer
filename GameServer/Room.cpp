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

void Room::Enter(GameObjectRef gameObject)
{
	bool successd = false;

	auto objectId = gameObject->_objectInfo->objectid();
	if (_gameObjects.find(objectId) != _gameObjects.end())
		successd = false;
	else
	{
		_gameObjects.insert(make_pair(objectId, gameObject));

		//player->_room.store(GetRoomRef());
		gameObject->_room = GetRoomRef();

		gameObject->_positionInfo->set_x(Utils::GetRandom(0.f, 50.f));
		gameObject->_positionInfo->set_y(0);
		gameObject->_positionInfo->set_z(Utils::GetRandom(0.f, 50.f));
		gameObject->_positionInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
		successd = true;
	}

	//	Room Enter ������ ���ο� �÷��̾�� ����
	{
		if (gameObject->_isPlayer)
		{
			Protocol::S_ENTER_GAME enterGamePacket;
			enterGamePacket.set_successed(successd);

			//	��� ����� ���� ���� �ִ�.
			auto objectInfo = new Protocol::ObjectInfo();
			objectInfo->CopyFrom(*gameObject->_objectInfo);

			//	enterGamePacket �Ҹ� �� playerInfo* ����
			//	������ ���� ���������� �Ϲ� ������ó�� �������� �������� �ʾƵ� ����Ʈ������ó�� �˾Ƽ� �Ҹ���
			enterGamePacket.set_allocated_player(objectInfo);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
		

			auto player = static_pointer_cast<Player>(gameObject);
			auto session = player->_ownerSession;
			if (session)
				session->Send(sendBuffer);
		}
	}

	//	���ο� �÷��̾��� ������ ���� �÷��̾�鿡�� ����
	{
		Protocol::S_SPAWN spawnPacket;

		auto playerInfo = spawnPacket.add_objects();
		playerInfo->CopyFrom(*gameObject->_objectInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);

		//	�� �ڽ��� ����
		//	�� �ڽ��� �̹� S_ENTER_GAME Packet���� ������ ���۹���
		Broadcast(sendBuffer, objectId);
	}

	//	���ο� �÷��̾�� ������ ���ӵǾ��ִ� �÷��̾� ����� ����
	{
		if (gameObject->_isPlayer)
		{
			Protocol::S_SPAWN spawnPacket;

			for (auto& gameObject : _gameObjects)
			{
				//	�� �ڽ��� ����
				//	�� �ڽ��� �̹� S_ENTER_GAME Packet���� ������ ���۹���
				auto inGameObject = gameObject.second;
				if (inGameObject->_objectInfo->objectid() == objectId)
					continue;

				auto addPlayer = spawnPacket.add_objects();
				addPlayer->CopyFrom(*inGameObject->_objectInfo);
			}

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);

			auto player = static_pointer_cast<Player>(gameObject);
			auto session = player->_ownerSession;
			if (session)
				session->Send(sendBuffer);
		}
	}
}

void Room::Leave(uint64 gameObjectId)
{
	//	�θ޸� ���� ����
	auto gameObject = _gameObjects.find(gameObjectId);
	if (gameObject == _gameObjects.end())
		return;

	if (gameObject->second->_isPlayer)
	{
		//	TEMP
		auto player = static_pointer_cast<Player>(gameObject->second);

		{
			//	������ ���� ���ο��� ����
			Protocol::S_LEAVE_GAME leaveGamePacket;
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePacket);

			auto session = player->_ownerSession;

			//	��...
			if (session)
				session->Send(sendBuffer);
		}
	}

	{
		//	������ ������ id�� �������ִµ� �������� ����
		Protocol::S_DESPAWN despawnPacket;
		despawnPacket.add_objectids(gameObjectId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPacket);

		//	�� �ڽ��� ����
		//	�� �ڽ��� �̹� S_LEAVE_GAME Packet���� ������ ���۹���
		//	�������� ��������?
		Broadcast(sendBuffer, gameObjectId);

		//if (auto session = player->_ownerSession.lock())
		//	session->Send(sendBuffer);
	}

	gameObject->second->_room = weak_ptr<Room>();
	_gameObjects.erase(gameObjectId);
}

void Room::Move(Protocol::C_MOVE movePacket)
{
	//	TODO : player ID check 
	const auto objectId = movePacket.positioninfo().objectid();
	if (_gameObjects.find(objectId) == _gameObjects.end())
		return;

	auto& gameObject = _gameObjects[objectId];
	//	TODO : Position Check
	//	Update Position
	gameObject->_positionInfo->CopyFrom(movePacket.positioninfo());

	{
		Protocol::S_MOVE moveSendPacket;
		auto positionInfo = moveSendPacket.mutable_positioninfo();
		positionInfo->CopyFrom(movePacket.positioninfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(moveSendPacket);
		Broadcast(sendBuffer);
	}
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 ignoreId)
{
	for (auto& gameObject : _gameObjects)
	{
		auto objectId = gameObject.second->_objectInfo->objectid();

		if (objectId == ignoreId)
			continue;

		if (gameObject.second->_isPlayer)
		{
			auto player = static_pointer_cast<Player>(gameObject.second);
			auto session = player->_ownerSession;

			//	�����̳� �ݺ��� �߿� Send�� �ϰ� �ִµ�
			//	� �̽��� ���� _players �����̳ʿ� ���� or ���� �̽��� ����ٸ�?
			if (session)
				session->Send(sendBuffer);
		}
	}
}

void Room::Update()
{
	//	Packet handle Processing per frame
	Execute(true);
}


