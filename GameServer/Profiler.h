#pragma once

class Performance
{
public:
	Performance()
	{
		::QueryPerformanceCounter(&_start);
	}
	Performance(const std::string& funcName)
	{
		::QueryPerformanceCounter(&_start);
		cout << funcName << " Func ";
	}
	~Performance()
	{
		::QueryPerformanceCounter(&_end);
		_deltaTime = (_end.QuadPart - _start.QuadPart) / (float)_frequency.QuadPart;
		cout << "Elapsed Time : " << _deltaTime * 1000 << "ms" << endl;
	}

public:
	static void Initialize()
	{
		::QueryPerformanceFrequency(&_frequency);
	}

private:
	static LARGE_INTEGER _frequency;
	LARGE_INTEGER		_start = { 0, };
	LARGE_INTEGER		_end = { 0, };
	float				_deltaTime = 0.f;
};

LARGE_INTEGER Performance::_frequency;