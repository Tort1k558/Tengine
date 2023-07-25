#pragma once
#include<array>
#include<vector>
#include<functional>
#include<unordered_map>
#include"Keys.h"
enum class EventType {
	MouseMoved,
	MouseButtonPressed,
	MouseButtonReleased,

	KeyPressed,
	KeyReleased,

	WindowClose,
	WindowResize
};

class Event
{
public:
	Event(EventType type) : m_type(type) {};
	virtual ~Event() = default;
	virtual EventType getType()
	{
		return m_type;
	}
protected:
	EventType m_type;
};

class EventDispatcher
{
public:
	EventDispatcher();
	template<typename T>
	void addEvent(std::function<void(T&)> callback)
	{
		auto baseCallback = [callback](Event& event)
		{
			if (event.getType() == T::type)
			{
				callback(static_cast<T&>(event));
			}
		};
		m_events[T::type].push_back(baseCallback);
	}
	void proccess(Event& event);
private:
	std::unordered_map<EventType, std::vector<std::function<void(Event&)>>> m_events;
};

class EventKeyPressed : public Event
{
public:
	EventKeyPressed(KeyCode code,bool repeat = false);
	static const EventType type = EventType::KeyPressed;
	KeyCode code;
	bool repeat;
};

class EventKeyReleased : public Event
{
public:
	EventKeyReleased(KeyCode code);
	static const EventType type = EventType::KeyReleased;
	KeyCode code;
};

class EventMouseMoved : public Event
{
public:
	EventMouseMoved(double x, double y);
	static const EventType type = EventType::MouseMoved;
	double x;
	double y;
};

class EventMouseButtonPressed : public Event
{
public:
	EventMouseButtonPressed(MouseButton code, double x, double y);
	MouseButton code;
	double x;
	double y;
	static const EventType type = EventType::MouseButtonPressed;
};

class EventMouseButtonReleased : public Event
{
public:
	EventMouseButtonReleased(MouseButton code, double x, double y);

	MouseButton code;
	double x;
	double y;
	static const EventType type = EventType::MouseButtonReleased;
};

class EventWindowClose : public Event
{
public:
	EventWindowClose();
	static const EventType type = EventType::WindowClose;
};

class EventWindowResize : public Event
{
public:
	EventWindowResize(unsigned int x, unsigned int y);
	unsigned int x;
	unsigned int y;
	static const EventType type = EventType::WindowResize;
};