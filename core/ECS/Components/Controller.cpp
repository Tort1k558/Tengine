#include "Controller.h"
namespace Tengine
{

	void Controller::addKeyCallback(KeyCode code, std::function<void(std::shared_ptr<Object>)> callback)
	{
		m_keyCallbacks.push_back({ code,callback });
	}

	void Controller::setMouseMoveCallback(std::function<void(std::shared_ptr<Object>)> callback)
	{
		m_mouseMoveCallback = callback;
	}

	std::vector<std::pair<KeyCode, std::function<void(std::shared_ptr<Object>)>>> Controller::getKeyCallbacks() const
	{
		return m_keyCallbacks;
	}

	std::function<void(std::shared_ptr<Object>)> Controller::getMouseMoveCallback() const
	{
		return m_mouseMoveCallback;
	}
}