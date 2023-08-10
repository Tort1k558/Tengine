#pragma once

#include"Scene/Scene.h"
#include"Core/Logger.h"
#include"Core/Core.h"

namespace Tengine
{

	class Scene;
	class TENGINE SceneManager
	{
	public:
		SceneManager() = delete;
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

		static std::shared_ptr<Scene> GetCurrentScene();
		static void SetCurrentScene(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> m_currentScene;
	private:
	};
}