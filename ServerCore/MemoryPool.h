#pragma once

enum
{
	SLIST_ALIGNMENT = 16
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryBlock : public SLIST_ENTRY
{
	MemoryBlock(uint32 size, uint32 threadId)
		: size(size), threadId(threadId)
	{

	}

	//	TODO : template
	static void* WriteMemoryBlockInfo(MemoryBlock* memory, uint32 size, uint32 threadId)
	{
		new(memory)MemoryBlock(size, threadId);
		return reinterpret_cast<void*>(++memory);
	}

	static MemoryBlock* GetAssignmentAddress(void* ptr)
	{
		auto memory = reinterpret_cast<MemoryBlock*>(ptr) - 1;
		return memory;
	}

	uint32 size = 0;
	uint32 threadId = 0;
};


DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(uint32 size);
	~MemoryPool();

public:
	void			Push(MemoryBlock* memory);
	MemoryBlock*	Pop();

private:
	SLIST_HEADER	_header;
	uint32			_size = 0;
};


class ThreadMemoryPoolCache
{
public:
	ThreadMemoryPoolCache() = default;
	~ThreadMemoryPoolCache()
	{
		if (_memoryPoolCache.size() > 0)
		{
			for (auto& memoryPoolCache : _memoryPoolCache)
			{
				for (auto memoryBlock : memoryPoolCache.second)
					_aligned_free(memoryBlock);
			}
		}

		_memoryPoolCache.clear();

	}

public:
	void Push(MemoryBlock* memory, uint32 size)
	{
		_memoryPoolCache[size].push_back(memory);
	}

	MemoryBlock* Pop(uint32 size)
	{
		auto memoryBlock = _memoryPoolCache[size].back();
		_memoryPoolCache[size].pop_back();
		return memoryBlock;
	}

public:
	uint32 GetThreadMemoryPoolCacheSize(uint32 size)
	{
		auto memoryPool = _memoryPoolCache.find(size);
		if (memoryPool == _memoryPoolCache.end())
			return 0;

		return static_cast<uint32>(memoryPool->second.size());
	}

private:
	std::unordered_map<uint32, std::vector<MemoryBlock*>> _memoryPoolCache;
};