#pragma once

extern class ThreadManager*		GThreadManager;
extern class CenterFreeList*	GMemoryPool;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue*		GGlobalQueue;
extern class JobTimer*			GJobTimer;
extern class DBConnectionPool*	GDBConnectionPool;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();

public:
	void Clear();
};

extern CoreGlobal* GCoreGlobal;