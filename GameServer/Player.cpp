#include "pch.h"
#include "Player.h"

Player::Player()
{
	//	TODO
	_info = GAllocate<Protocol::PlayerInfo>();
}

Player::~Player()
{
	GDeallocate(_info);
}
