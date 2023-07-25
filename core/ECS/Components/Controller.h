#pragma once

#include<functional>
#include<vector>

#include"ECS/Component.h"
#include"Core/Keys.h"

class Controller : public Component
{
public:
	Controller() = default;
	void addKeyCallback(KeyCode code, std::function<void(std::shared_ptr<Object>)> callback);
	void setMouseCallback(std::function<void(std::shared_ptr<Object>)> callback);
	std::vector<std::pair<KeyCode, std::function<void(std::shared_ptr<Object>)>>> getKeyCallbacks() { return m_callbacks; }
	std::function<void(std::shared_ptr<Object>)> getMouseCallback() { return m_mouseCallback; }
private:
	std::vector<std::pair<KeyCode, std::function<void(std::shared_ptr<Object>)>>> m_callbacks;
	std::function<void(std::shared_ptr<Object>)> m_mouseCallback;
};