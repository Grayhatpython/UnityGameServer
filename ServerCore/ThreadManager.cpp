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

		//	��� ���� �ð����� �ϰ��� ó������
		if (now > LEndTickCount)
			break;

		//	GlobalQueue���� ó�� ����ϰ� �ִ� JobQueue�� ���´�.
		auto jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		//	�� ������� JobQueue�� ó���Ѵ�.
		jobQueue->Execute();
	}
}

//	����� Job�� ���
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
