#pragma once
#include "Types.h"


/*
class Lock
{
    enum : uint32
    {
        ACQUIRE_TIMEOUT_TICK = 10000,
        MAX_SPIN_COUNT = 5000,
        WRITE_THREAD_MASK = 0xFFFF'0000,
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };

public:
    void WriteLock(const char* name);
    void WriteUnlock(const char* name);
    void ReadLock(const char* name);
    void ReadUnlock(const char* name);

private:
    Atomic<uint32> _lockFlag = EMPTY_FLAG;
    uint16 _writeCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
    const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
    const char* _name;
};
*/

class Lock
{
	enum
	{
		EMPTY_OWNER_THREAD_ID = 0,
		LOCK_SPIN_COUNT = 1024,
	};

public:
	Lock()
	{
		::InitializeSRWLock(&_lock);
	}

public:
	void WriteLock();
	void WriteUnLock();

private:
	SRWLOCK			_lock;
	atomic<uint32>	_ownerThreadId = 0;
	atomic<uint32>	_nestedCount = 0;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock)
		: _lock(lock)
	{
		_lock.WriteLock();
	}
	~WriteLockGuard()
	{
		_lock.WriteUnLock();
	}

private:
	Lock& _lock;
};
