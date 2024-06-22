#include "pch.h"
#include "Profiler.h"

//Profiler GProfiler;

void Profiler::Initialize(uint32 recordSize, uint32 maxEventCount)
{
	Shutdown();

	_eventData = new EventData[recordSize];
	_recordSize = recordSize;

	for (uint32 i = 0; i < recordSize; ++i)
		_eventData[i].allEventsOftheFrame.resize(maxEventCount);
}

void Profiler::Shutdown()
{
	if (_eventData)
	{
		delete[] _eventData;
		_eventData = nullptr;
	}
}

void Profiler::BeginRecordingEvent(const char* name, const char* filePath, uint32 lineNumber)
{
	if (_isPaused)
		return;

	auto& eventData = GetEventData();
	//	first 0
	uint32 newEventIndex = eventData.numOfEvents.fetch_add(1);
	ASSERT_CRASH(newEventIndex < eventData.allEventsOftheFrame.size());

	auto& threadEventTrace = GetThreadEventTrace();

	auto& newEvent = eventData.allEventsOftheFrame[newEventIndex];
	newEvent.depth = threadEventTrace.threadEventStack.Size();
	newEvent.threadIndex = threadEventTrace.threadIndex;
	newEvent.name = eventData.allocator.CopyStringAndRet(name);
	newEvent.filePath = filePath;
	newEvent.lineNumber = lineNumber;

	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&newEvent.beginTick));

	threadEventTrace.threadEventStack.Push() = newEventIndex;
}

void Profiler::EndRecordingEvent()
{
	if (_isPaused)
		return;

	auto& eventData = GetEventData();
	auto eventIndex = GetThreadEventTrace().threadEventStack.Pop();
	auto& event = eventData.allEventsOftheFrame[eventIndex];

	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&event.beginTick));
}

void Profiler::Tick()
{
	_isPaused = _isQueuedPaused;

	if (_isPaused)
		return;

	if (_frameIndex)
		EndRecordingEvent();

	for (auto& threadData : _threadsData)
		ASSERT_CRASH(threadData.LthreadEventTrace->threadEventStack.Size() == 0);

	auto& eventData = GetEventData();
	auto& events = eventData.allEventsOftheFrame;

	std::sort(events.begin(), events.begin() + eventData.numOfEvents, [](const Event& a, const Event& b)
		{
			return a.threadIndex < b.threadIndex;
		});

	Range eventRange{ 0,0 };
	for (uint32 threadIndex = 0; threadIndex < static_cast<uint32>(_threadsData.size()); ++threadIndex)
	{
		while (threadIndex < events[eventRange.begin].threadIndex)
			eventRange.begin++;

		eventRange.end = eventRange.begin;

		while (events[eventRange.end].threadIndex == threadIndex && eventRange.end < eventData.numOfEvents)
			eventRange.end++;

		//	TEMP
		eventData.eventsPerThreadOftheFrame[threadIndex].clear();
		for (auto i = eventRange.begin; i < eventRange.end - eventRange.begin; i++)
			eventData.eventsPerThreadOftheFrame[threadIndex].push_back(events[i]);

		eventRange.begin = eventRange.end;
	}

	_frameIndex++;

	auto& newEventData = GetEventData();
	newEventData.allocator.Clean();
	newEventData.numOfEvents.store(0);

	BeginRecordingEvent("CPU Frame");
}

void Profiler::RegisterThread(const char* name)
{
	auto& LthreadEventTrace = GetThreadEventTranceTLS();
	ASSERT_CRASH(LthreadEventTrace.isInitialized == false);
	LthreadEventTrace.isInitialized = true;

	std::scoped_lock lock(_lock);
	LthreadEventTrace.threadIndex = static_cast<uint32>(_threadsData.size());
	auto& threadData = _threadsData.emplace_back();

	// If the name is not provided, retrieve it using GetThreadDescription()
	if (name)
		strcpy_s(threadData.name, ARRAYSIZE(threadData.name), name);
	else
	{
		PWSTR threadDescription = nullptr;
		::GetThreadDescription(::GetCurrentThread(), &threadDescription);
		size_t converted = 0;
		ASSERT_CRASH(::wcstombs_s(&converted, threadData.name, ARRAYSIZE(threadData.name), threadDescription, ARRAYSIZE(threadData.name)) == 0);
	}
	threadData.threadId = ::GetCurrentThreadId();
	threadData.LthreadEventTrace = &LthreadEventTrace;
	threadData.index = static_cast<uint32>(_threadsData.size()) - 1;

	for (uint32 i = 0; i < _recordSize; i++)
		_eventData[i].eventsPerThreadOftheFrame.resize(_threadsData.size());
}

Range Profiler::GetFrameRange() const
{
	uint32 begin = _frameIndex - min(_frameIndex, _recordSize) + 1;
	uint32 end = _frameIndex;

	return Range{begin, end};
}

std::vector<Event> Profiler::GetEventsForThread(const ThreadData& threadData, uint32 frame) const
{
	ASSERT_CRASH(frame >= GetFrameRange().begin && frame < GetFrameRange().end);
	auto& eventData = _eventData[frame % _recordSize];
	if (threadData.index < eventData.eventsPerThreadOftheFrame.size())
		return eventData.eventsPerThreadOftheFrame[threadData.index];

	return {};
}

void Profiler::GetRecordRange(uint64& minTick, uint64& maxTick) const
{
	auto range = GetFrameRange();
	minTick = GetEventData(range.begin).allEventsOftheFrame[0].beginTick;
	minTick = GetEventData(range.end).allEventsOftheFrame[0].endTick;
}
