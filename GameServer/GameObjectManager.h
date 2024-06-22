#pragma once

class GameObjectManager
{
public:
	static PlayerRef CreatePlayer(ClientSessionRef session);

private:
	static Atomic<uint64> S_autoincrementId;
};

