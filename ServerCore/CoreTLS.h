#pragma once

extern thread_local uint32						LThreadId;
extern thread_local uint64						LEndTickCount;
extern thread_local SendBufferChunkRef			LSendBufferChunk;
extern thread_local class JobQueueBasedObject*	LCurrentExecuteJobQueue;
extern thread_local class ThreadMemoryPoolCache* LThreadMemoryPoolCache;

//	TEMP
extern class ThreadMemoryPoolCache* GThreadMemoryPoolCacheTable[6];