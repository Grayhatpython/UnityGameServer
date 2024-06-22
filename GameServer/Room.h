#pragma once

#include "Protocol.pb.h"

class Room : public JobQueueBasedObject
{
public:
	Room();
	virtual ~Room();

public:
	//	TEMP : Main Thread
	//	싱글스레드 환경

	//	Recv Packet Process Func
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer, uint64 ignoreId = 0);

public:
	void Update();

public:
	RoomRef GetRoomRef() { return static_pointer_cast<Room>(shared_from_this()); }

private:
	unordered_map<uint64, PlayerRef> _players;
};

//	TEMP
extern RoomRef GRoom;
