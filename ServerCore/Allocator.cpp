#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

void* Allocator::Allocate(uint32 size)
{
	return GMemoryPool->Allocate(size);
}

void Allocator::Deallocate(void* ptr)
{
	GMemoryPool->Deallocate(ptr);
}