#pragma once

#include<unordered_map>
#include<filesystem>

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

		static std::shared_ptr<Scene> GetCurrentScene();
		static void SetCurrentScene(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> m_currentScene;
		static void Save(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> LoadByPath(std::filesystem::path path);
		static std::shared_ptr<Scene> LoadByName(const std::string& name);
		static void AddScene(const std::string& name, std::filesystem::path path);
	private:
		static std::unordered_map<std::string, std::filesystem::path> m_sceneNames;
		static void DeserializeField(nlohmann::json& data, std::shared_ptr<FieldInfo> element);
		static void SerializeField(nlohmann::json& data, std::shared_ptr<FieldInfo> element);
	};
}