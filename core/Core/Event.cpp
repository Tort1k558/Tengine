#include "Event.h"

namespace Tengine
{

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

	const EventType EventMouseMoved::type = EventType::MouseMoved;
	const EventType EventKeyPressed::type = EventType::KeyPressed;
	const EventType EventKeyReleased::type = EventType::KeyReleased;
	const EventType EventMouseButtonPressed::type = EventType::MouseButtonPressed;
	const EventType EventMouseButtonReleased::type = EventType::MouseButtonReleased;
	const EventType EventWindowClose::type = EventType::WindowClose;
	const EventType EventWindowResize::type = EventType::WindowResize;

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
}