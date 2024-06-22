#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

/*
void Lock::WriteLock(const char* name)
{

	// 동일한 쓰레드가 소유하고 있다면 무조건 성공.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻는다.
	const int64 beginTick = ::GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			ASSERT_CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnlock(const char* name)
{

	// ReadLock 다 풀기 전에는 WriteUnlock 불가능.
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
		ASSERT_CRASH("INVALID_UNLOCK_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{

	// 동일한 쓰레드가 소유하고 있다면 무조건 성공.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			ASSERT_CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock(const char* name)
{

	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		ASSERT_CRASH("MULTIPLE_UNLOCK");
}
*/

void Lock::WriteLock()
{
	//	현재 WriteLock을 잡고 있는 ThreadId를 확인
	auto threadId = _ownerThreadId.load();

	//	현재 WriteLock을 잡고 있는 ThreadId와 지금 WriteLock을 하는 ThreadId가 같으면 통과
	//	중첩 Lock 카운트 증가
	if (threadId == LThreadId)
	{
		_nestedCount.fetch_add(1);
		return;
	}

	//	지금 호출한 Thread가 아닌 다른 Thread가 WriteLock을 걸고 있는 경우
	while (true)
	{
		uint32 expected = EMPTY_OWNER_THREAD_ID;
		if (_ownerThreadId.compare_exchange_strong(OUT expected, LThreadId))
		{
			//	지금 호출한 Thread가 WriteLock
			::AcquireSRWLockExclusive(&_lock);
			//	호출한 ThreadId를 ownerThreadId에 저장
			_ownerThreadId.store(LThreadId);
			//	중첩 Lock 카운트 증가
			_nestedCount.fetch_add(1);
			return;
		}

		for (auto i = 0; i < LOCK_SPIN_COUNT; i++)
		{
			//YieldProcessor();
			std::this_thread::yield();
		}
	}
}

void Lock::WriteUnLock()
{
	//	현재 WriteLock을 잡고 있는 ThreadId를 확인
	auto threadId = _ownerThreadId.load();

	//	현재 WriteLock을 잡고 있는 ThreadId와 지금 WriteLock을 하는 ThreadId가 같으면 통과
	//	중첩 Lock 카운트 증가
	if (threadId == LThreadId)
	{
		auto prevCount = _nestedCount.fetch_sub(1);

		if (prevCount == 1)
		{
			::ReleaseSRWLockExclusive(&_lock);
			_ownerThreadId.store(EMPTY_OWNER_THREAD_ID);
		}
	}
}