#include "SceneSerializer.h"

#include<nlohmann/json.hpp>
#include<fstream>

#include"Core/Logger.h"
#include"Scene/SceneManager.h"
#include"Components/Camera.h"
#include"Components/Mesh.h"
#include"Components/Transform.h"
#include"Utils/Primitives.h"

namespace Tengine
{

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
				
				if (dataObject.contains("transform"))
				{
					std::shared_ptr<Transform> transform = Component::Create<Transform>();
					transform->setPosition(Vec3(dataObject["transform"]["position"][0], dataObject["transform"]["position"][1], dataObject["transform"]["position"][2]));
					transform->setRotation(Vec3(dataObject["transform"]["rotation"][0], dataObject["transform"]["rotation"][1], dataObject["transform"]["rotation"][2]));
					transform->setScale(Vec3(dataObject["transform"]["scale"][0], dataObject["transform"]["scale"][1], dataObject["transform"]["scale"][2]));

					object->addComponent<Transform>(transform);
				}

				if (dataObject.contains("camera"))
				{
					std::shared_ptr<Camera> camera;
					switch (static_cast<ProjectionType>(dataObject["camera"]["projectionType"]))
					{
					case ProjectionType::Perspective:
					{
						camera = Component::Create<Camera>(ProjectionType::Perspective);
						camera->setRotationOrder(static_cast<RotationOrder>(dataObject["camera"]["rotationOrder"]));
						std::shared_ptr<PerspectiveProjection> perspective = camera->getPerspectiveProjection();
						perspective->setZNear(dataObject["camera"]["perspective"]["zNear"]);
						perspective->setZFar(dataObject["camera"]["perspective"]["zFar"]);
						perspective->setFov(dataObject["camera"]["perspective"]["fov"]);
						perspective->setAspectRatio(dataObject["camera"]["perspective"]["aspectRatio"]);
						break;
					}
					case ProjectionType::Orthographical:
					{
						camera = Component::Create<Camera>(ProjectionType::Orthographical);
						camera->setRotationOrder(static_cast<RotationOrder>(dataObject["camera"]["rotationOrder"]));
						std::shared_ptr<OrthographicalProjection> orthographical = camera->getOrthographicalProjection();
						orthographical->setZNear(dataObject["camera"]["orthographical"]["zNear"]);
						orthographical->setZFar(dataObject["camera"]["orthographical"]["zFar"]);
						orthographical->setLeft(dataObject["camera"]["orthographical"]["left"]);
						orthographical->setRight(dataObject["camera"]["orthographical"]["right"]);
						orthographical->setBottom(dataObject["camera"]["orthographical"]["bottom"]);
						orthographical->setTop(dataObject["camera"]["orthographical"]["top"]);
						break;
					}
					default:
						break;
					}
					object->addComponent<Camera>(camera);
				}

				if (dataObject.contains("mesh"))
				{
					std::shared_ptr<Mesh> mesh;
					if (dataObject["mesh"].contains("path"))
					{
						std::string path = dataObject["mesh"]["path"];
						if (path == "Primitive::Quad")
						{
							mesh = Primitives::CreateQuad();
						}
						else if (path == "Primitive::Cube")
						{
							mesh = Primitives::CreateCube();
						}
						else if (path.find("Primitive::Sphere::") != std::string::npos)
						{
							size_t sectorsPos = path.find("::Sectors::");
							size_t stacksPos = path.rfind("::Stacks::");

							int sectors;
							int stacks;

							if (sectorsPos == std::string::npos || stacksPos == std::string::npos || sectorsPos == stacksPos) {
								sectors = 1;
								stacks = 1;
							}
							sectors = std::stoi(path.substr(sectorsPos + 11, stacksPos - sectorsPos - 11));
							stacks = std::stoi(path.substr(stacksPos + 10));
							mesh = Primitives::CreateSphere(sectors, stacks);
						}
						else
						{
							mesh = AssetManager::LoadMesh(path);
						}
					}
					object->addComponent<Mesh>(mesh);
				}
			}
		}
		return scene;
	}
}