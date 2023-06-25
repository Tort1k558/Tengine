#include "Event.h"

EventDispatcher::EventDispatcher() :
	m_events()
{

}

void EventDispatcher::proccess(Event& event)
{
	auto events = m_events[event.getType()];
	for (const auto& el : events)
	{
		el(event);
	}
}

EventMouseMoved::EventMouseMoved(double x, double y) :
	Event(type), x(x), y(y)
{

}

EventKeyPressed::EventKeyPressed(KeyCode code) :
	Event(type), code(code)
{

}

EventKeyReleased::EventKeyReleased(KeyCode code) :
	Event(type), code(code)
{

}

EventMouseButtonPressed::EventMouseButtonPressed(MouseCode code, double x, double y) :
	Event(type), code(code), x(x), y(y)
{

}

EventMouseButtonReleased::EventMouseButtonReleased(MouseCode code, double x, double y) :
	Event(type), code(code), x(x), y(y)
{

}

EventWindowClose::EventWindowClose() :
	Event(type)
{

}

EventWindowResize::EventWindowResize(unsigned int x, unsigned int y) :
	Event(type), x(x), y(y)
{

}
