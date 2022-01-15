#include "source.h"
#include "EventHandler.h"

#include <iostream>

Application* g_app = nullptr;
EventBus g_eventBus;

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

int main() 
{
	EventBus* bus = new EventBus();

	Reciever* reciever = new Reciever();
	bus->subscribe(reciever, &Reciever::PrintMsg);
	bus->publish(new MessageEvent("hi"));

	delete bus;

	g_app = new Application();
	g_app->Init();


	return 0;
}