#include "ControllerSystem.h"

#include"ECS/Components/Controller.h"
#include"Scene/SceneManager.h"
#include"Core/Input.h"

namespace Tengine
{
	std::shared_ptr<ControllerSystem> ControllerSystem::m_instance;

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

	std::shared_ptr<ControllerSystem> ControllerSystem::GetInstance()
	{
		if (!m_instance)
		{
			m_instance = std::make_shared<ControllerSystem>();
		}
		return m_instance;
	}
}