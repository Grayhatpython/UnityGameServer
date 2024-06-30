#pragma once

#include "Protocol.pb.h"

class Room : public JobQueueBasedObject
{
public:
	Room();
	virtual ~Room();

public:
	//	Recv Packet Process Func
	void Enter(GameObjectRef gameObject);
	void Leave(uint64 gameObjectId);
	void Move(Protocol::C_MOVE movePacket);
	void Broadcast(SendBufferRef sendBuffer, uint64 ignoreId = 0);

public:
	//	TEMP : Main Thread
	//	싱글스레드 환경
	void Update();

public:
	RoomRef GetRoomRef() { return static_pointer_cast<Room>(shared_from_this()); }

private:
	unordered_map<uint64, GameObjectRef> _gameObjects;
};

//	TEMP
extern RoomRef GRoom;
