#include "ControllerSystem.h"

#include"ECS/Components/Controller.h"
#include"Scene/SceneManager.h"
#include"Core/Input.h"

void ControllerSystem::init()
{
}

void ControllerSystem::update()
{
	std::vector<std::shared_ptr<Controller>> controllers = SceneManager::GetCurrentScene()->getComponents<Controller>();
	for (auto& controller : controllers)
	{
		auto callbacks = controller->getKeyCallbacks();
		std::shared_ptr<Object> parent = controller->getParent();
		for (auto& callback : callbacks)
		{
			if (Input::IsKeyPressed(callback.first))
			{
				callback.second(parent);
			}
		}
		auto mouseCallback = controller->getMouseMoveCallback();
		if (mouseCallback)
		{
			mouseCallback(parent);
		}
	}
}

void ControllerSystem::destroy()
{
}
