#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	//	TODO : �޸�Ǯ
	_objectInfo = new Protocol::ObjectInfo();
	_positionInfo = new Protocol::PositionInfo();

	_objectInfo->set_allocated_positioninfo(_positionInfo);
}

GameObject::~GameObject()
{
	delete _objectInfo;
}
