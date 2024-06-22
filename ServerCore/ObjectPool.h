#pragma once
#include "Types.h"
#include "MemoryPool.h"

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
		Type* memory = static_cast<Type*>(MemoryBlock::WriteMemoryBlockInfo(s_pool.Pop(), s_allocSize, LThreadId));
		new(memory)Type(forward<Args>(args)...); // placement new
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
		s_pool.Push(MemoryBlock::GetAssignmentAddress(obj));
	}

	template<typename... Args>
	static shared_ptr<Type> MakeShared(Args&&... args)
	{
		shared_ptr<Type> ptr = { Pop(forward<Args>(args)...), Push };
		return ptr;
	}

private:
	static uint32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
uint32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryBlock);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };