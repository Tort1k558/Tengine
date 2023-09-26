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
		static void Save(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> Load(std::filesystem::path path);
	private:
		static void DeserializeField(nlohmann::json& data, std::shared_ptr<FieldInfo> element);
		static void SerializeField(nlohmann::json& data, std::shared_ptr<FieldInfo> element);
	};
}