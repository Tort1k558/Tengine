#include "SceneSerializer.h"

#include<nlohmann/json.hpp>
#include<fstream>

#include"Core/Logger.h"
#include"Scene/SceneManager.h"
#include"ECS/Components/Camera.h"
void SceneSerializer::Serialize(std::shared_ptr<Scene> scene)
{
	//Prepare data
    nlohmann::json data;
	data["name"] = scene->getName();
	for (const auto& object : scene->getAllObjects())
	{
		data[object->getId().string()]["name"] = object->getName();
		if (object->hasComponent<Transform>())
		{
			std::shared_ptr<Transform> transform = object->getComponent<Transform>();
			Vec3 position = transform->getPosition();
			data[object->getId().string()]["transform"]["position"] = { position.x,position.y,position.z };
			Vec3 rotation = transform->getRotation();
			data[object->getId().string()]["transform"]["rotation"] = { rotation.x,rotation.y,rotation.z };
			Vec3 scale = transform->getScale();
			data[object->getId().string()]["transform"]["scale"] = { scale.x,scale.y,scale.z };
		}
		if (object->hasComponent<Camera>())
		{
			std::shared_ptr<Camera> camera = object->getComponent<Camera>();

			data[object->getId().string()]["camera"]["rotationOrder"] = camera->getRotationOrder();
			data[object->getId().string()]["camera"]["projectionType"] = camera->getProjectionType();
			switch (camera->getProjectionType())
			{
			case ProjectionType::Perspective:
			{
				std::shared_ptr<PerspectiveProjection> perspective = camera->getPerspectiveProjection();
				data[object->getId().string()]["camera"]["perspective"]["zNear"] = perspective->getZNear();
				data[object->getId().string()]["camera"]["perspective"]["zFar"] = perspective->getZFar();
				data[object->getId().string()]["camera"]["perspective"]["fov"] = perspective->getFov();
				data[object->getId().string()]["camera"]["perspective"]["aspectRatio"] = perspective->getAspectRatio();
				break;
			}
			case ProjectionType::Orthographical:
			{
				std::shared_ptr<OrthographicalProjection> orthographical = camera->getOrthographicalProjection();
				data[object->getId().string()]["camera"]["orthographical"]["zNear"] = orthographical->getZNear();
				data[object->getId().string()]["camera"]["orthographical"]["zFar"] = orthographical->getZFar();
				data[object->getId().string()]["camera"]["orthographical"]["left"] = orthographical->getLeft();
				data[object->getId().string()]["camera"]["orthographical"]["right"] = orthographical->getRight();
				data[object->getId().string()]["camera"]["orthographical"]["bottom"] = orthographical->getBottom();
				data[object->getId().string()]["camera"]["orthographical"]["top"] = orthographical->getTop();
				break;
			}
			default:
				break;
			}

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
			std::string objectName = item.value()["name"];
			object->setName(objectName);
			if (item.value().contains("transform"))
			{
				std::shared_ptr<Transform> transform = Component::Create<Transform>();
				transform->setPosition(Vec3(item.value()["transform"]["position"][0], item.value()["transform"]["position"][1], item.value()["transform"]["position"][2]));
				transform->setRotation(Vec3(item.value()["transform"]["rotation"][0], item.value()["transform"]["rotation"][1], item.value()["transform"]["rotation"][2]));
				transform->setScale(Vec3(item.value()["transform"]["scale"][0], item.value()["transform"]["scale"][1], item.value()["transform"]["scale"][2]));

				object->addComponent<Transform>(transform);
			}
			if (item.value().contains("camera"))
			{
				std::shared_ptr<Camera> camera;
				switch (static_cast<ProjectionType>(item.value()["camera"]["projectionType"]))
				{
				case ProjectionType::Perspective:
				{
					camera = Component::Create<Camera>(ProjectionType::Perspective);
					camera->setRotationOrder(static_cast<RotationOrder>(item.value()["camera"]["rotationOrder"]));
					std::shared_ptr<PerspectiveProjection> perspective = camera->getPerspectiveProjection();
					perspective->setZNear(item.value()["camera"]["perspective"]["zNear"]);
					perspective->setZFar(item.value()["camera"]["perspective"]["zFar"]);
					perspective->setFov(item.value()["camera"]["perspective"]["fov"]);
					perspective->setAspectRatio(item.value()["camera"]["perspective"]["aspectRatio"]);
					break;
				}
				case ProjectionType::Orthographical:
				{
					camera = Component::Create<Camera>(ProjectionType::Orthographical);
					camera->setRotationOrder(static_cast<RotationOrder>(item.value()["camera"]["rotationOrder"]));
					std::shared_ptr<OrthographicalProjection> orthographical = camera->getOrthographicalProjection();
					orthographical->setZNear(item.value()["camera"]["orthographical"]["zNear"]);
					orthographical->setZFar(item.value()["camera"]["orthographical"]["zFar"]);
					orthographical->setLeft(item.value()["camera"]["orthographical"]["left"]);
					orthographical->setRight(item.value()["camera"]["orthographical"]["right"]);
					orthographical->setBottom(item.value()["camera"]["orthographical"]["bottom"]);
					orthographical->setTop(item.value()["camera"]["orthographical"]["top"]);
					break;
				}
				default:
					break;
				}
				object->addComponent<Camera>(camera);
			}
		}
	}
	return scene;
}
