#pragma once

#include<memory>
#include<filesystem>

#include"Scene/Scene.h"
#include"Core/Core.h"

namespace Tengine
{

	class TENGINE SceneSerializer
	{
	public:
		SceneSerializer() = delete;
		SceneSerializer(const SceneSerializer&) = delete;
		SceneSerializer(SceneSerializer&&) = delete;
		SceneSerializer& operator=(const SceneSerializer&) = delete;
		SceneSerializer& operator=(SceneSerializer&&) = delete;

		static void Serialize(std::shared_ptr<Scene> scene);
		static std::shared_ptr<Scene> Deserialize(std::filesystem::path path);
	};
}