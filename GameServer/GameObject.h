#pragma once

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

public:

public:
	bool					_isPlayer = false;

	//	TODO
	Protocol::ObjectInfo*	_objectInfo = nullptr;
	Protocol::PositionInfo* _positionInfo = nullptr;

	//	MultiThread 
	//	Two Block
	//	������ �ٲ� �� �ִ� ����Ʈ������
	//Atomic<weak_ptr<Room>>		_room;
	weak_ptr<Room>			_room;
};

