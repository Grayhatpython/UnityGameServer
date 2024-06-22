#pragma once

#include "Allocator.h"

template<typename T, uint32 N>
class ThreadEventFixedStack
{
public:
	T& Pop()
	{
		ASSERT_CRASH(_depth > 0);
		_depth--;
		return _stack[_depth];
	}

	T& Push()
	{
		_depth++;
		ASSERT_CRASH(_depth < ARRAYSIZE(_stack));
		return _stack[_depth - 1];
	}

	T& Top()
	{
		ASSERT_CRASH(_depth > 0);
		return _stack[_depth - 1];
	}

public:
	uint32	Size() const { return _depth; }

private:
	uint32	_depth = 0;
	T		_stack[N] = {};
};

//	Thread local storage
struct LThreadEventTrace
{
	static constexpr int32 MAX_STACK_DEPTH = 32;

	ThreadEventFixedStack<uint32, MAX_STACK_DEPTH>	threadEventStack;
	uint32											threadIndex = 0;
	bool											isInitialized = false;
};

struct ThreadData
{
	char				name[MAX_PATH] = { 0, };
	uint32				threadId = 0;
	uint32				index = 0;
	LThreadEventTrace*	LthreadEventTrace = nullptr;
};

struct Event
{
	const char* name = ""; 
	const char* filePath = nullptr;

	uint64		beginTick = 0;
	uint64		endTick = 0;

	uint16		lineNumber = 0;
	uint16		threadIndex = 0;
	uint16		depth = 0;
};

struct EventData
{
	static constexpr uint32 MAX_ALLOC_SIZE = 0x4000;

	EventData()
		: allocator(MAX_ALLOC_SIZE)
	{

	}

	std::vector<std::vector<Event>>		eventsPerThreadOftheFrame;
	std::vector<Event>					allEventsOftheFrame;
	Allocator							allocator;	//	Data storage allocator that dynamically allocates data during a frame.
	Atomic<uint32>						numOfEvents = 0;

};

struct Range
{
	uint32 begin = 0;
	uint32 end = 0;
};

class Profiler
{
public:
	void Initialize(uint32 recordSize, uint32 maxEventCount);
	void Shutdown();

public:
	void BeginRecordingEvent(const char* name, const char* filePath = nullptr, uint32 lineNumber = 0);
	void EndRecordingEvent();

public:
	void Tick();	
	void RegisterThread(const char* name = nullptr);

public:
	Range						GetFrameRange() const;
	std::vector<Event>			GetEventsForThread(const ThreadData& threadData, uint32 frame) const;
	void						GetRecordRange(uint64& minTick, uint64& maxTick) const;

public:
	std::vector<ThreadData>		GetThreadsData() const { return _threadsData; }
	void						SetPaused(bool paused) { _isQueuedPaused = paused; }
	bool						IsPaused() const { return _isPaused; }

private:
	static LThreadEventTrace& GetThreadEventTranceTLS()
	{
		static thread_local LThreadEventTrace LthreadEventTrace;
		return LthreadEventTrace;
	}

	LThreadEventTrace& GetThreadEventTrace()
	{
		auto& LthreadEventTrace = GetThreadEventTranceTLS();
		if (LthreadEventTrace.isInitialized == false)
			RegisterThread();

		return LthreadEventTrace;
	}

	EventData& GetEventData() { return GetEventData(_frameIndex); }
	EventData& GetEventData(uint32 frameIndex) { return _eventData[_frameIndex % _recordSize]; }
	const EventData& GetEventData(uint32 frameIndex) const { return _eventData[_frameIndex % _recordSize]; } 
	
private:
	Mutex					_lock;
	std::vector<ThreadData>	_threadsData;
	EventData* _eventData = nullptr;
	uint32 _recordSize = 0;
	uint32 _frameIndex = 0;
	bool	_isPaused = false;
	bool	_isQueuedPaused = false;
};

extern Profiler GProfiler;

class ProfilerScope
{
public:
	ProfilerScope(const char* funcName, const char* filePath, uint32 lineNumber, const char* name)
	{
		GProfiler.BeginRecordingEvent(name, filePath, lineNumber);
	}		

	ProfilerScope(const char* funcName, const char* filePath, uint32 lineNumber)
	{
		GProfiler.BeginRecordingEvent(funcName, filePath, lineNumber);
	}

	~ProfilerScope()
	{
		GProfiler.EndRecordingEvent();
	}
};

#define MACRO_CONCAT( x, y ) x##y

#define PROFILE_FRAME()			GProfiler.Tick();
#define PROFILE_SCOPE(...)		ProfilerScope MACRO_CONCAT(profilerScope, __COUNTER__)(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)