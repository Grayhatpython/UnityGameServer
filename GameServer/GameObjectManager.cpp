#include "pch.h"
#include "GameObjectManager.h"
#include "ClientSession.h"
#include "Room.h"
#include "Player.h"

Atomic<uint64> GameObjectManager::S_autoincrementId = 1;

PlayerRef GameObjectManager::CreatePlayer(ClientSessionRef session)
{
	const auto id = S_autoincrementId.fetch_add(1);

	auto player = MakeShared<Player>();
	player->_objectInfo->set_objectid(id);
	player->_positionInfo->set_objectid(id);
	player->_ownerSession = session;
	//session->_player.store(player);
	session->_player = player;

	return player;
}
