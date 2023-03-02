#pragma once
#include <iostream>

template<
	typename TReturnType,
	typename TCallable,
	typename TBaseType,
	typename ...TDispatchableTypes>
class SingleDispatcher;

template<
	typename TReturnType,
	typename TCallable,
	typename TBaseType>
class SingleDispatcher<
	TReturnType,
	TCallable,
	TBaseType>
{
protected:
	TCallable callable;
public:
	SingleDispatcher<
		TReturnType,
		TCallable,
		TBaseType>
		(TCallable callable) : callable(std::move(callable)) {};
	TReturnType Dispatch(TBaseType const* dispatchable)
	{
		return TReturnType{};
	}
};

template<
	typename TReturnType,
	typename TCallable,
	typename TBaseType,
	typename TDispatchableType,
	typename ...TDispatchableTypes>
class SingleDispatcher<
	TReturnType,
	TCallable,
	TBaseType,
	TDispatchableType,
	TDispatchableTypes...>
	: public SingleDispatcher<
	TReturnType,
	TCallable,
	TBaseType,
	TDispatchableTypes...>
{
public:
	SingleDispatcher<
		TReturnType,
		TCallable,
		TBaseType,
		TDispatchableType,
		TDispatchableTypes...>(TCallable callable) :
		SingleDispatcher<
		TReturnType,
		TCallable,
		TBaseType,
		TDispatchableTypes...>(std::move(callable)) {};

	TReturnType Dispatch(TBaseType const* dispatchable)
	{
		TDispatchableType const* target = dynamic_cast<TDispatchableType const*>(dispatchable);

		if (target)
		{
			return SingleDispatcher<
				TReturnType,
				TCallable,
				TBaseType,
				TDispatchableTypes...
			>::callable(target);
		}

		return SingleDispatcher<
			TReturnType,
			TCallable,
			TBaseType,
			TDispatchableTypes...
		>::Dispatch(dispatchable);
	}
};