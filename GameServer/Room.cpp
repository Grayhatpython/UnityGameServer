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

	//	Room Enter 성공을 새로운 플레이어에게 전송
	{
		if (gameObject->_isPlayer)
		{
			Protocol::S_ENTER_GAME enterGamePacket;
			enterGamePacket.set_successed(successd);

			//	사실 방식은 여러 개가 있다.
			auto objectInfo = new Protocol::ObjectInfo();
			objectInfo->CopyFrom(*gameObject->_objectInfo);

			//	enterGamePacket 소멸 시 playerInfo* 삭제
			//	성능은 조금 포기하지만 일반 포인터처럼 수동으로 관리하지 않아도 스마트포인터처럼 알아서 소멸함
			enterGamePacket.set_allocated_player(objectInfo);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
		

			auto player = static_pointer_cast<Player>(gameObject);
			auto session = player->_ownerSession;
			if (session)
				session->Send(sendBuffer);
		}
	}

	//	새로운 플레이어의 접속을 기존 플레이어들에게 전송
	{
		Protocol::S_SPAWN spawnPacket;

		auto playerInfo = spawnPacket.add_objects();
		playerInfo->CopyFrom(*gameObject->_objectInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);

		//	나 자신은 제외
		//	나 자신은 이미 S_ENTER_GAME Packet으로 정보를 전송받음
		Broadcast(sendBuffer, objectId);
	}

	//	새로운 플레이어에게 기존에 접속되어있던 플레이어 목록을 전송
	{
		if (gameObject->_isPlayer)
		{
			Protocol::S_SPAWN spawnPacket;

			for (auto& gameObject : _gameObjects)
			{
				//	나 자신은 제외
				//	나 자신은 이미 S_ENTER_GAME Packet으로 정보를 전송받음
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
	//	인메모리 삭제 과정
	auto gameObject = _gameObjects.find(gameObjectId);
	if (gameObject == _gameObjects.end())
		return;

	if (gameObject->second->_isPlayer)
	{
		//	TEMP
		auto player = static_pointer_cast<Player>(gameObject->second);

		{
			//	나가는 유저 본인에게 전송
			Protocol::S_LEAVE_GAME leaveGamePacket;
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePacket);

			auto session = player->_ownerSession;

			//	흠...
			if (session)
				session->Send(sendBuffer);
		}
	}

	{
		//	나가는 유저의 id를 접속해있는데 유저에게 전송
		Protocol::S_DESPAWN despawnPacket;
		despawnPacket.add_objectids(gameObjectId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPacket);

		//	나 자신은 제외
		//	나 자신은 이미 S_LEAVE_GAME Packet으로 정보를 전송받음
		//	이중으로 받을지는?
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

			//	컨테이너 반복문 중에 Send를 하고 있는데
			//	어떤 이슈로 인해 _players 컨테이너에 삭제 or 삽입 이슈가 생긴다면?
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


