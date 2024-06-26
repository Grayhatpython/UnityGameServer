#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "GlobalQueue.h"

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();

			while(_stopped == false)
				callback();

			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);

	LThreadMemoryPoolCache = new ThreadMemoryPoolCache();
	//GThreadMemoryPoolCacheTable[LThreadId] = LThreadMemoryPoolCache;
}

void ThreadManager::DestroyTLS()
{
	if (LThreadMemoryPoolCache)
		delete LThreadMemoryPoolCache;
}

void ThreadManager::DoGlobalJobQueueExecute()
{
	while (true)
	{
		uint64 now = ::GetTickCount64();

		//	어느 정도 시간동안 일감을 처리할지
		if (now > LEndTickCount)
			break;

		//	GlobalQueue에서 처리 대기하고 있는 JobQueue를 뺴온다.
		auto jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		//	그 대기중인 JobQueue를 처리한다.
		jobQueue->Execute();
	}
}

//	예약된 Job들 배분
void ThreadManager::DistributeReserveJobs()
{
	const uint64 now = ::GetTickCount64();

	GJobTimer->Distribute(now);
}

void ThreadManager::Close()
{
	_stopped.store(true); 
	DestroyTLS();
}
