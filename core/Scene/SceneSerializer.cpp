#include "SceneSerializer.h"

#include<nlohmann/json.hpp>
#include<fstream>

#include"Core/Logger.h"
#include"Scene/SceneManager.h"
#include"ECS/Components/Camera.h"
#include"ECS/Components/Mesh.h"
#include"ECS/Components/Transform.h"
#include"Utils/Primitives.h"
#include"Utils/Reflection.hpp"

void SceneSerializer::Serialize(std::shared_ptr<Scene> scene)
{
	//Prepare data
    nlohmann::json data;
	data["name"] = scene->getName();
	for (const auto& object : scene->getAllObjects())
	{
		data[object->getId().string()]["name"] = object->getName();
		for (const auto& component : object->getComponents())
		{
			component->serialize(data[object->getId().string()]);
		}
	}

	//Write data to file
	std::ofstream file(scene->getName() + ".json");
	if (file.is_open()) {
		file << data.dump(4);
		file.close();
	}
	else {
		Logger::Critical("ERROR::Serializer::Failed to open the file for writing!");
	}
}

std::shared_ptr<Scene> SceneSerializer::Deserialize(std::filesystem::path path)
{
	std::shared_ptr<Scene> scene = Scene::Create();
	SceneManager::SetCurrentScene(scene);
	std::ifstream file(path);
	nlohmann::json data = nlohmann::json::parse(file);
	scene->setName(data["name"].get<std::string>());
	for (const auto& item : data.items())
	{
		if (item.key() != "name")
		{
			std::string objectId = item.key();
			std::shared_ptr<Object> object = Object::Create(UUID(objectId));
			nlohmann::json dataObject = item.value();
			if (dataObject.contains("name"))
			{
				object->setName(dataObject["name"]);
			}
			reflang::Deserialize(dataObject, object);
		}
	}
	return scene;
}
