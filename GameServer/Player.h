#pragma once
#include "Creature.h"

class Room;
class ClientSession;

class Player : public Creature
{
public:
	Player();
	virtual ~Player();

public:
	ClientSessionRef			_ownerSession;
};

