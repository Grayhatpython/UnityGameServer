#include "pch.h"
#include "CoreTLS.h"

thread_local uint32					LThreadId = 0;
thread_local uint64					LEndTickCount = 0;
thread_local SendBufferChunkRef		LSendBufferChunk;
thread_local JobQueueBasedObject*	LCurrentExecuteJobQueue = nullptr;
thread_local ThreadMemoryPoolCache* LThreadMemoryPoolCache = nullptr;

ThreadMemoryPoolCache* GThreadMemoryPoolCacheTable[6] = { nullptr, };