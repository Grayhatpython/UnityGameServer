#pragma once

class Room;
class ClientSession;

class Player : public std::enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

public:
	//	TODO
	ClientSessionRef			_ownerSession;
	Protocol::PlayerInfo*		_info = nullptr;

public:
	//	MultiThread 
	//	Two Block
	//	언제라도 바뀔 수 있는 스마트포인터
	//Atomic<weak_ptr<Room>>		_room;
	weak_ptr<Room>		_room;
};

