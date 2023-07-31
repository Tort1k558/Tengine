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
	void setMouseMoveCallback(std::function<void(std::shared_ptr<Object>)> callback);
	std::vector<std::pair<KeyCode, std::function<void(std::shared_ptr<Object>)>>> getKeyCallbacks();
	std::function<void(std::shared_ptr<Object>)> getMouseMoveCallback();
private:
	std::vector<std::pair<KeyCode, std::function<void(std::shared_ptr<Object>)>>> m_keyCallbacks;
	std::function<void(std::shared_ptr<Object>)> m_mouseMoveCallback;
};