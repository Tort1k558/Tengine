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

EventKeyPressed::EventKeyPressed(KeyCode code, bool repeat) :
	Event(type), code(code), repeat(repeat)
{

}

EventKeyReleased::EventKeyReleased(KeyCode code) :
	Event(type), code(code)
{

}

EventMouseButtonPressed::EventMouseButtonPressed(MouseButton code, double x, double y) :
	Event(type), code(code), x(x), y(y)
{

}

EventMouseButtonReleased::EventMouseButtonReleased(MouseButton code, double x, double y) :
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