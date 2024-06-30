#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	//	TODO : 메모리풀
	_objectInfo = new Protocol::ObjectInfo();
	_positionInfo = new Protocol::PositionInfo();

	_objectInfo->set_allocated_positioninfo(_positionInfo);
}

GameObject::~GameObject()
{
	delete _objectInfo;
}
