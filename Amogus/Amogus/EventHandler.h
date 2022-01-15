#pragma once

#include <list>
#include <string>
#include <map>
#include <typeindex>

class Event
{
protected:
	~Event() {};
};

class HandlerFunctionBase
{
public:
	virtual ~HandlerFunctionBase() {};

	void exec(Event* evnt) 
	{ 
		call(evnt); 
	}

private:
	virtual void call(Event*) = 0;
};

template<class T, class EventType> 
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void (T::*MemberFunction)(EventType*);

	MemberFunctionHandler(T* instance, MemberFunction memberFunction) : m_instance(instance), m_function(memberFunction) {};

	void call(Event* evnt)
	{
		(m_instance->*m_function)(static_cast<EventType*>(evnt));
	}

private:
	T* m_instance;

	MemberFunction m_function;
};

typedef std::list<HandlerFunctionBase*> HandlerList;
class EventBus
{
public:

	template<typename EventType>
	void publish(EventType* evnt)
	{
		HandlerList* handlers = m_subscribers[typeid(EventType)];

		if (handlers == nullptr)
			return;

		for (auto& handler : *handlers)
		{
			if (handler != nullptr)
				handler->exec(evnt);
		}
	}


	template<class T, class EventType>
	void subscribe(T* instance, void (T::* memberFunction)(EventType*))
	{
		HandlerList* handlers = m_subscribers[typeid(EventType)];

		if (handlers == nullptr)
		{
			handlers = new HandlerList();
			m_subscribers[typeid(EventType)] = handlers;
		}

		handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
	}

private:
	std::map<std::type_index, HandlerList*> m_subscribers;
};