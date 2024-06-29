#pragma once

struct ThreadCallFunctionFrame
{
	std::string		functionName;
	std::string		fileName;
	int				line = 0;
	LARGE_INTEGER	start = { 0, };
};

class ThreadProfile
{

public:
	void PushCallFunctionFrame(const std::string& functionName, const std::string& fileName, int line)
	{
		ThreadCallFunctionFrame frame;
		frame.functionName = functionName;
		frame.fileName = fileName;
		frame.line = line;
	}

	void PopCallFunctionFrame()
	{

	}

private:
	std::stack<ThreadCallFunctionFrame>			_threadCallFunctionsStack;
	std::vector<std::pair<std::string, float>>	_callFunctionsElapsedTime;
};

struct TimeMeasurement
{
public:
	void Measurement()
	{
		::QueryPerformanceCounter(&_start);
	}

	void End()
	{
		::QueryPerformanceCounter(&_end);
		_elapsedTime = (_end.QuadPart - _start.QuadPart) / (float)_frequency.QuadPart;
		//cout << "Elapsed Time : " << _deltaTime * 1000 << "ms" << endl;
	}

public:
	float ElapsedTimeMs() const { return _elapsedTime; }

public:
	static void Initialize()
	{
		::QueryPerformanceFrequency(&_frequency);
	}

public:
	static LARGE_INTEGER	_frequency;
	LARGE_INTEGER			_start = { 0, };
	LARGE_INTEGER			_end = { 0, };
	float					_elapsedTime = 0.f;
};