#include "Allocator.h"
#pragma once
#include "Types.h"
#include "CoreMacro.h"

class Allocator
{
public:
	Allocator(uint32 size)
		: _data(new char[size]), _size(size)
	{

	}

	~Allocator()
	{
		if (_data)
		{
			delete[] _data;
			_data = nullptr;
		}
	}

	void Clean()
	{
		_offset.store(0);
	}

public:
	template<typename T, typename... Args>
	T* Allocate(Args... args)
	{
		void* ptr = Allocate(sizeof(T));
		T* ret = new(ptr)T(std::forward<Args>(args)...);
		return ret;
	}

	void* Allocate(uint32 size)
	{
		uint32 offset = _offset.fetch_add(size);
		ASSERT_CRASH(offset + size <= _size);
		return _data + offset;
	}

	inline void Allocator::Deallocate(void* ptr)
	{
	}

public:
	const char* CopyStringAndRet(const char* src)
	{
		auto len = static_cast<uint32>(::strlen(src)) + 1;
		char* dest = reinterpret_cast<char*>(Allocate(len));
		::strcpy_s(dest, len, src);
		return dest;
	}

private:
	char*			_data = nullptr;
	uint32			_size = 0;
	Atomic<uint32>	_offset = 0;
};