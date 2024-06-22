#pragma once
#include <functional>

/*
template<int... Remains>
struct seq
{};

template<int N,int... Remains>
struct gen_seq : gen_seq<N - 1, N - 1, Remains...>
{};

template<int... Remains>
struct gen_seq<0, Remains...> : seq<Remains...>
{};

template<typename Ret, typename... Args>
void xapply(Ret(*func)(Args...), std::tuple<Args...>& tup)
{
	xapply_helper(func, gen_seq<sizeof...(Args)>(), tup);
}

template<typename F, typename... Args, int... ls>
void xapply_helper(F func, seq<ls...>, std::tuple<Args...>& tup)
{
	(func)(std::get<ls>(tup)...);
}

template<typename T,typename Ret, typename... Args>
void xapply(T* obj, Ret(T::*func)(Args...), std::tuple<Args...>& tup)
{
	xapply_helper(obj, func, gen_seq<sizeof...(Args)>(), tup);
}

template<typename T, typename F, typename... Args, int... ls>
void xapply_helper(T* obj, F func, seq<ls...>, std::tuple<Args...>& tup)
{
	(obj->*func)(std::get<ls>(tup)...);
}


class IJob
{
public:
	virtual void Execute() abstract;
};

template<typename Ret, typename... Args>
class FuncJob : public IJob
{
	using FuncType = Ret(*)(Args...);

public:
	FuncJob(FuncType func, Args... args)
		: _func(func), _tuple(args...)
	{

	}
	virtual void Execute() override
	{
		xapply(_func, _tuple);
	}

private:
	FuncType			_func;
	std::tuple<Args...> _tuple;
};

template<typename T, typename Ret, typename... Args>
class MemberJob : public IJob
{
	using FuncType = Ret(T::*)(Args...);

public:
	MemberJob(T* obj, FuncType func, Args... args)
		: _obj(obj), _func(func), _tuple(args...)
	{

	}
	virtual void Execute() override
	{
		xapply(_obj,_func, _tuple);
	}

private:
	T*					_obj = nullptr;
	FuncType			_func;
	std::tuple<Args...> _tuple;
};

*/

using CallbackType = std::function<void()>;

class Job
{
public:
	Job(CallbackType&& callback)
		: _callback(std::move(callback))
	{

	}
	template<typename T, typename Ret, typename... Args>
	Job(shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		//	Ref Count Áõ°¡
		_callback = [owner, memFunc, args...]()
		{
			(owner.get()->*memFunc)(args...);
		};
	}

public:
	void Execute()
	{
		_callback();
	}

private:
	CallbackType _callback;
};
