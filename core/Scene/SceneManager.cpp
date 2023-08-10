#include "SceneManager.h"

namespace Tengine
{

	std::shared_ptr<Scene> SceneManager::m_currentScene;

	void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
	{
		m_currentScene = scene;
	}
	std::shared_ptr<Scene> SceneManager::GetCurrentScene()
	{
		return m_currentScene;
	}
}