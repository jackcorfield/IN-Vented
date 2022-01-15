#pragma once

#include <list>
#include <string>
#include <map>
#include <typeindex>

/*
EXAMPLE USAGE:

// Defined somewhere

class MessageEvent : public Event
{
public:
	MessageEvent(std::string msg) : m_msg(msg) {}
	std::string m_msg;
};

class Reciever
{
public:
	Reciever() {}

	void PrintMsg(MessageEvent* e)
	{
		std::cout << e->m_msg << std::endl;
	}
};

// Put this somewhere
EventBus* bus = new EventBus();

Reciever* reciever = new Reciever();
bus->subscribe(reciever, &Reciever::PrintMsg);
bus->publish(new MessageEvent("Hello world!"));

delete bus;

*/

// Base event class
// Descendents of this class will be what get pushed across when we publish an event
class Event
{
protected:
	~Event() {};
};

// This is just an abstract interface for the subscriber
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

// This is what we view a subscriber to be
// It's essentially just an instance, and the function that we want to call
// Just so we remember who to call
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

// Objects can "subscribe" to the event bus
// So when something happens they're interested in, when the bus is "published", they'll be sent the published event
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