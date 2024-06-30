#include "pch.h"
#include "Creature.h"

Creature::Creature()
{
	_objectInfo->set_objecttype(Protocol::ObjectType::OBJECT_TYPE_CREATURE);
}

Creature::~Creature()
{
}
