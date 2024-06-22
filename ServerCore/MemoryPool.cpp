#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(uint32 size) : _size(size)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryBlock* memory = static_cast<MemoryBlock*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

void MemoryPool::Push(MemoryBlock* memory)
{
	memory->size = 0;
	memory->threadId = 0;

	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(memory));
}

MemoryBlock* MemoryPool::Pop()
{
	auto memory = static_cast<MemoryBlock*>(::InterlockedPopEntrySList(&_header));

	if (memory == nullptr)
		memory = reinterpret_cast<MemoryBlock*>(::_aligned_malloc(_size, SLIST_ALIGNMENT));

	return memory;
}