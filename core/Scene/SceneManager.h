#pragma once

#include"Scene/Scene.h"

class Scene;
class SceneManager
{
public:
	SceneManager() = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static void SetCurrentScene(std::shared_ptr<Scene> scene);
	static std::shared_ptr<Scene> GetCurrentScene() { return m_currentScene; }
private:
	static std::shared_ptr<Scene> m_currentScene;
};