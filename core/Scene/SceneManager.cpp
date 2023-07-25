#include "SceneManager.h"

std::shared_ptr<Scene> SceneManager::m_currentScene;

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_currentScene = scene;
}
