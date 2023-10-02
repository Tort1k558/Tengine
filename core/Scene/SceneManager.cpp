#include "SceneManager.h"

#include<fstream>
#include<nlohmann/json.hpp>
#include"Components/Camera.h"
#include"Components/Mesh.h"
#include"Components/Transform.h"
#include"Utils/Primitives.h"
#include"Systems/ScriptSystem.h"
#include"Components/Script.h"

namespace Tengine
{

	std::shared_ptr<Scene> SceneManager::m_currentScene;
	std::unordered_map<std::string, std::filesystem::path> SceneManager::m_sceneNames;

	void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
	{
		m_currentScene = scene;
	}
	void SceneManager::Save(std::shared_ptr<Scene> scene)
	{
		//Prepare data
		nlohmann::json data;
		data["name"] = scene->getName();
		for (const auto& object : scene->getAllObjects())
		{
			data[object->getId().string()]["name"] = object->getName();
			for (const auto& component : object->getComponents())
			{
				ComponentInfo info = component->getInfo();
				auto elements = info.getElements();
				auto& componentData = data[object->getId().string()][info.getComponentName()];
				for (size_t i = 0; i < elements.size(); i++)
				{
					SerializeField(componentData[elements[i]->name], elements[i]);
				}
			}
		}

		//Write data to file
		scene->setPath(scene->getPath().parent_path().string() + "/" + scene->getName() + scene->getPath().extension().string());
		std::ofstream file(scene->getPath().string(),std::ios_base::out);
		if (file.is_open()) {
			file << data.dump(4);
			file.close();
		}
		else {
			Logger::Critical("ERROR::SceneManager::Failed to save the scene file!");
		}
	}

	std::shared_ptr<Scene> SceneManager::LoadByPath(std::filesystem::path path)
	{
		std::shared_ptr<Scene> scene = Scene::Create();
		SetCurrentScene(scene);
		std::ifstream file(path.string());
		if (file.is_open())
		{
			nlohmann::json data = nlohmann::json::parse(file);
			scene->setPath(path);
			scene->setName(data["name"].get<std::string>());
			for (const auto& item : data.items())
			{
				if (item.key() != "name")
				{
					std::string objectId = item.key();
					std::shared_ptr<Object> object = Object::Create(UUID(objectId));
					nlohmann::json dataObject = item.value();
					for (auto it = dataObject.begin(); it != dataObject.end(); ++it)
					{
						if (it.key() == "name")
						{
							object->setName(it.value());
						}
						else if (it.key() == "Transform")
						{
							std::shared_ptr<Transform> transform = object->getComponent<Transform>();
							ComponentInfo info = transform->getInfo();
							auto elements = info.getElements();
							auto& componentData = dataObject[info.getComponentName()];
							for (size_t i = 0; i < elements.size(); i++)
							{
								DeserializeField(componentData[elements[i]->name], elements[i]);
							}
						}
						else if (it.key() == "Camera")
						{
							std::shared_ptr<Camera> camera;
							camera = Component::Create<Camera>();
							ComponentInfo info = camera->getInfo();
							auto elements = info.getElements();
							auto& componentData = dataObject[info.getComponentName()];
							for (size_t i = 0; i < elements.size(); i++)
							{
								DeserializeField(componentData[elements[i]->name], elements[i]);
							}
							object->addComponent(camera);
						}
						else if (it.key() == "Mesh")
						{
							std::shared_ptr<Mesh> mesh = Component::Create<Mesh>();
							ComponentInfo info = mesh->getInfo();
							auto elements = info.getElements();
							auto& componentData = dataObject[info.getComponentName()];
							for (size_t i = 0; i < elements.size(); i++)
							{
								DeserializeField(componentData[elements[i]->name], elements[i]);
							}
							object->addComponent(mesh);
						}
						else
						{
							std::vector<std::string> nameScripts = ScriptSystem::GetInstance()->getScriptNames();
							for (size_t i = 0; i < nameScripts.size(); i++)
							{
								if (nameScripts[i] == it.key())
								{
									std::shared_ptr<Component> script = ScriptSystem::GetInstance()->addScript(object, it.key());
									ComponentInfo info = script->getInfo();
									auto elements = info.getElements();
									auto& componentData = dataObject[info.getComponentName()];
									for (size_t i = 0; i < elements.size(); i++)
									{
										DeserializeField(componentData[elements[i]->name], elements[i]);
									}
								}
							}
						}
					}
				}
			}
			file.close();
		}
		else
		{
			Logger::Critical("ERROR::SceneManager::Failed to open the scene file!");
		}
		return scene;
	}

	std::shared_ptr<Scene> SceneManager::LoadByName(std::string_view name)
	{
		std::shared_ptr<Scene> scene;
		if (m_sceneNames.find(name.data()) != m_sceneNames.end())
		{
			scene = LoadByPath(m_sceneNames[name.data()]);
		}
		else
		{
			Logger::Critical("ERROR::SceneManager::Failed to load the scene, scene name {0} doesn't exists", name);
			scene = Scene::Create();
			SetCurrentScene(scene);
		}

		return scene;
	}
	void SceneManager::AddScene(std::string_view name, std::filesystem::path path)
	{
		m_sceneNames.insert({ name.data(), path});
	}
	void SceneManager::DeserializeField(nlohmann::json& data,std::shared_ptr<FieldInfo> element)
	{
		switch (element->type)
		{
		case FieldType::Float:
		{
			std::shared_ptr<FieldFloat> field = std::dynamic_pointer_cast<FieldFloat>(element);
			*field->data = static_cast<float>(data[0]);
			break;
		}
		case FieldType::Vec2:
		{
			std::shared_ptr<FieldVec2> field = std::dynamic_pointer_cast<FieldVec2>(element);
			*field->data = Vec2(data[0], data[1]);
			break;
		}
		case FieldType::Vec3:
		{
			std::shared_ptr<FieldVec3> field = std::dynamic_pointer_cast<FieldVec3>(element);
			*field->data = Vec3(data[0], data[1], data[2]);
			break;
		}
		case FieldType::Vec4:
		{
			std::shared_ptr<FieldVec4> field = std::dynamic_pointer_cast<FieldVec4>(element);
			*field->data = Vec4(data[0], data[1], data[2], data[3]);
			break;
		}
		case FieldType::Enum:
		{
			std::shared_ptr<FieldEnum> field = std::dynamic_pointer_cast<FieldEnum>(element);
			*field->currentElement = data;
			field->callback(*field->currentElement);
			break;
		}
		case FieldType::File:
		{
			std::shared_ptr<FieldFile> field = std::dynamic_pointer_cast<FieldFile>(element);
			field->callback(data);
			break;
		}
		default:
			break;
		}
	}

	void SceneManager::SerializeField(nlohmann::json& data, std::shared_ptr<FieldInfo> element)
	{
		switch (element->type)
		{
		case FieldType::Float:
		{
			std::shared_ptr<FieldFloat> field = std::dynamic_pointer_cast<FieldFloat>(element);
			data = {*field->data};
			break;
		}
		case FieldType::Vec2:
		{
			std::shared_ptr<FieldVec2> field = std::dynamic_pointer_cast<FieldVec2>(element);
			data = { field->data->x,field->data->y };
			break;
		}
		case FieldType::Vec3:
		{
			std::shared_ptr<FieldVec3> field = std::dynamic_pointer_cast<FieldVec3>(element);
			data = { field->data->x,field->data->y,field->data->z };
			break;
		}
		case FieldType::Vec4:
		{
			std::shared_ptr<FieldVec4> field = std::dynamic_pointer_cast<FieldVec4>(element);
			data = { field->data->x,field->data->y,field->data->z,field->data->w };
			break;
		}
		case FieldType::Enum:
		{
			std::shared_ptr<FieldEnum> field = std::dynamic_pointer_cast<FieldEnum>(element);
			data = *field->currentElement;
			break;
		}
		case FieldType::File:
		{
			std::shared_ptr<FieldFile> field = std::dynamic_pointer_cast<FieldFile>(element);
			data = field->path;
			break;
		}
		default:
			break;
		}
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene()
	{
		return m_currentScene;
	}
}