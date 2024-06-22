#include "pch.h"
#include "CoreGlobal.h"
#include "SocketUtils.h"
#include "Memory.h"
#include "ThreadManager.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "DBConnectionPool.h"

ThreadManager*		GThreadManager = nullptr;
CenterFreeList*		GMemoryPool = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;
GlobalQueue*		GGlobalQueue = nullptr;
JobTimer*			GJobTimer = nullptr;
DBConnectionPool*	GDBConnectionPool = nullptr;

CoreGlobal* GCoreGlobal = new CoreGlobal();


CoreGlobal::CoreGlobal()
{
	SocketUtils::Init();
	GThreadManager = new ThreadManager();
	GMemoryPool = new CenterFreeList();
	GSendBufferManager = new SendBufferManager();
	GGlobalQueue = new GlobalQueue();
	GJobTimer = new JobTimer();
	GDBConnectionPool = new DBConnectionPool();
}

CoreGlobal::~CoreGlobal()
{
}


void CoreGlobal::Clear()
{

	if (GSendBufferManager)
	{
		delete GSendBufferManager;
		GSendBufferManager = nullptr;
	}
	if (GGlobalQueue)
	{
		delete GGlobalQueue;
		GGlobalQueue = nullptr;
	}
	if (GJobTimer)
	{
		delete GJobTimer;
		GJobTimer = nullptr;
	}
	if (GDBConnectionPool)
	{
		delete GDBConnectionPool;
		GDBConnectionPool = nullptr;
	}
	if (GMemoryPool)
	{
		delete GMemoryPool;
		GMemoryPool = nullptr;
	}

	GThreadManager->Close();
	GThreadManager->Join();

	if (GThreadManager)
	{
		delete GThreadManager;
		GThreadManager = nullptr;
	}

	SocketUtils::Clear();
}
