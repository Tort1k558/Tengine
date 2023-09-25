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
		std::ofstream file(scene->getName() + ".json");
		if (file.is_open()) {
			file << data.dump(4);
			file.close();
		}
		else {
			Logger::Critical("ERROR::Serializer::Failed to open the file for writing!");
		}
	}

	std::shared_ptr<Scene> SceneManager::Load(std::filesystem::path path)
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
		SetCurrentScene(scene);
		return scene;
	}
	void SceneManager::DeserializeField(nlohmann::json& data,std::shared_ptr<DisplayInfoElement> element)
	{
		switch (element->type)
		{
		case DisplayTypeElement::Slider:
		{
			std::shared_ptr<DisplayInfoElementSlider> field = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
			*field->data = static_cast<float>(data[0]);
			break;
		}
		case DisplayTypeElement::Slider2:
		{
			std::shared_ptr<DisplayInfoElementSlider2> field = std::dynamic_pointer_cast<DisplayInfoElementSlider2>(element);
			*field->data = Vec2(data[0], data[1]);
			break;
		}
		case DisplayTypeElement::Slider3:
		{
			std::shared_ptr<DisplayInfoElementSlider3> field = std::dynamic_pointer_cast<DisplayInfoElementSlider3>(element);
			*field->data = Vec3(data[0], data[1], data[2]);
			break;
		}
		case DisplayTypeElement::Slider4:
		{
			std::shared_ptr<DisplayInfoElementSlider4> field = std::dynamic_pointer_cast<DisplayInfoElementSlider4>(element);
			*field->data = Vec4(data[0], data[1], data[2], data[3]);
			break;
		}
		case DisplayTypeElement::Combo:
		{
			std::shared_ptr<DisplayInfoElementCombo> field = std::dynamic_pointer_cast<DisplayInfoElementCombo>(element);
			*field->currentElement = data;
			field->callback(*field->currentElement);
			break;
		}
		case DisplayTypeElement::FileDialog:
		{
			std::shared_ptr<DisplayInfoElementFileDialog> field = std::dynamic_pointer_cast<DisplayInfoElementFileDialog>(element);
			field->callback(data);
			break;
		}
		default:
			break;
		}
	}
	void SceneManager::SerializeField(nlohmann::json& data, std::shared_ptr<DisplayInfoElement> element)
	{
		switch (element->type)
		{
		case DisplayTypeElement::Slider:
		{
			std::shared_ptr<DisplayInfoElementSlider> field = std::dynamic_pointer_cast<DisplayInfoElementSlider>(element);
			data = {*field->data};
			break;
		}
		case DisplayTypeElement::Slider2:
		{
			std::shared_ptr<DisplayInfoElementSlider2> field = std::dynamic_pointer_cast<DisplayInfoElementSlider2>(element);
			data = { field->data->x,field->data->y };
			break;
		}
		case DisplayTypeElement::Slider3:
		{
			std::shared_ptr<DisplayInfoElementSlider3> field = std::dynamic_pointer_cast<DisplayInfoElementSlider3>(element);
			data = { field->data->x,field->data->y,field->data->z };
			break;
		}
		case DisplayTypeElement::Slider4:
		{
			std::shared_ptr<DisplayInfoElementSlider4> field = std::dynamic_pointer_cast<DisplayInfoElementSlider4>(element);
			data = { field->data->x,field->data->y,field->data->z,field->data->w };
			break;
		}
		case DisplayTypeElement::Combo:
		{
			std::shared_ptr<DisplayInfoElementCombo> field = std::dynamic_pointer_cast<DisplayInfoElementCombo>(element);
			data = *field->currentElement;
			break;
		}
		case DisplayTypeElement::FileDialog:
		{
			std::shared_ptr<DisplayInfoElementFileDialog> field = std::dynamic_pointer_cast<DisplayInfoElementFileDialog>(element);
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