#include "Controller.h"

void Controller::addKeyCallback(KeyCode code, std::function<void(std::shared_ptr<Object>)> callback)
{
	m_callbacks.push_back({ code,callback });
}

void Controller::setMouseCallback(std::function<void(std::shared_ptr<Object>)> callback)
{
	m_mouseCallback = callback;
}
