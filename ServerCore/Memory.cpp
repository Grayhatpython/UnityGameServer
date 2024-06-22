#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"


CenterFreeList::CenterFreeList()
{
	int32 size = 0;
	int32 memoryPoolIndex = 0;

	for (size = SMALL_OBJECT_SPLICT_SIZE; size < SMALL_OBJECT_STANDARD_SIZE; size += SMALL_OBJECT_SPLICT_SIZE)
	{
		MemoryPool* memoryPool = new MemoryPool(size);

		while (memoryPoolIndex <= size)
		{
			_memoryPools[memoryPoolIndex] = memoryPool;
			memoryPoolIndex++;
		}
	}

	for (size = SMALL_OBJECT_STANDARD_SIZE; size <= LARGE_OBJECT_STANDARD_SIZE; size += MEDIAM_OBJECT_SPLICT_SIZE)
	{
		MemoryPool* memoryPool = new MemoryPool(size);

		while (memoryPoolIndex <= size)
		{
			_memoryPools[memoryPoolIndex] = memoryPool;
			memoryPoolIndex++;
		}
	}
}


CenterFreeList::~CenterFreeList()
{
	int32 size = 0;

	/*for (auto i = 0; i < 6; i++)
	{
		if(GThreadMemoryPoolCacheTable[i])
			delete GThreadMemoryPoolCacheTable[i];
	}*/

	for (size = SMALL_OBJECT_SPLICT_SIZE; size < SMALL_OBJECT_STANDARD_SIZE; size += SMALL_OBJECT_SPLICT_SIZE)
	{
		auto memoryPool = _memoryPools[size];
		if (memoryPool)
			delete memoryPool;
	}

	for (size = SMALL_OBJECT_STANDARD_SIZE; size <= LARGE_OBJECT_STANDARD_SIZE; size += MEDIAM_OBJECT_SPLICT_SIZE)
	{
		auto memoryPool = _memoryPools[size];
		if (memoryPool)
			delete memoryPool;
	}


	_memoryPools.clear();
}

void* CenterFreeList::Allocate(uint32 size)
{
	MemoryBlock* memory = nullptr;
	uint32 allocSize = size + sizeof(MemoryBlock);

	if (allocSize > LARGE_OBJECT_STANDARD_SIZE)
		memory = reinterpret_cast<MemoryBlock*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	else
	{
		if (LThreadMemoryPoolCache)
		{
			auto threadMemoryPoolCacheSize = LThreadMemoryPoolCache->GetThreadMemoryPoolCacheSize(allocSize);
			if (threadMemoryPoolCacheSize > 0)
				memory = LThreadMemoryPoolCache->Pop(allocSize);
			else
			{
				for (int32 i = 0; i < THREAD_CACHE_CHUNK_SIZE; i++)
				{
					memory = _memoryPools[allocSize]->Pop();
					MemoryBlock::WriteMemoryBlockInfo(memory, allocSize, LThreadId);
					LThreadMemoryPoolCache->Push(memory, allocSize);
				}

				memory = LThreadMemoryPoolCache->Pop(allocSize);
			}
		}
		else
			memory = _memoryPools[allocSize]->Pop();
	}

	return MemoryBlock::WriteMemoryBlockInfo(memory, allocSize, LThreadId);
}

void CenterFreeList::Deallocate(void* ptr)
{
	MemoryBlock* memory = MemoryBlock::GetAssignmentAddress(ptr);

	uint32 allocSize = memory->size;

	if (allocSize > LARGE_OBJECT_STANDARD_SIZE)
		::_aligned_free(memory);
	else
	{
		//	TODO : allocSize를 고칠필요가?

		if (memory->threadId != LThreadId)
			memory->threadId = LThreadId;

		//if (threadMemoryPoolCache.size() > MAX_THREAD_CACHE_CHUNK_SIZE)
		//{
		//	//	Global Return
		//	while (threadMemoryPoolCache.size() < (MAX_THREAD_CACHE_CHUNK_SIZE / 2))
		//	{
		//		auto freeMemory = threadMemoryPoolCache.back();
		//		threadMemoryPoolCache.pop_back();
		//		_memoryPools[allocSize]->Push(freeMemory);
		//	}
		//}

		LThreadMemoryPoolCache->Push(memory, allocSize);
	}
}