#pragma once
#include "Allocator.h"

class MemoryPool;

class CenterFreeList
{
public:
	enum
	{
		SMALL_OBJECT_SPLICT_SIZE = 32,
		SMALL_OBJECT_STANDARD_SIZE = 1024,

		MEDIAM_OBJECT_SPLICT_SIZE = 128,

		LARGE_OBJECT_STANDARD_SIZE = 4096,

		THREAD_CACHE_CHUNK_SIZE = 10,
		MAX_THREAD_CACHE_CHUNK_SIZE = 150,
	};

public:
	CenterFreeList();
	~CenterFreeList();

public:
	void*	Allocate(uint32 size);
	void	Deallocate(void* ptr);

private:
	std::unordered_map<uint32, MemoryPool*>		_memoryPools;
};


template<typename T, typename... Args>
T* GAllocate(Args&&... args)
{
	T* memory = static_cast<T*>(GMemoryPool->Allocate(sizeof(T)));
	new(memory)T(std::forward<Args>(args)...);
	return memory;
}

template<typename T>
void GDeallocate(T* ptr)
{
	ptr->~T();
	GMemoryPool->Deallocate(ptr);
}

template<typename T, typename... Args>
shared_ptr<T> MakeShared(Args&&... args)
{
	return shared_ptr<T>{ GAllocate<T>(std::forward<Args>(args)...), GDeallocate<T> };
}