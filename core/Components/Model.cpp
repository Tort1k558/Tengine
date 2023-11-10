#include "Model.h"

#include<fstream>

#include"Core/Logger.h"
#include"Utils/Primitives.h"

namespace Tengine
{
	Model::Model(std::shared_ptr<Mesh> mesh)
	{
		setMesh(mesh);
	}
	std::shared_ptr<Mesh> Model::getMesh() const
	{
		return m_mesh;
	}

	void Model::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
		save();
	}

	void Model::setSubmeshMaterial(size_t index, std::shared_ptr<Material> material)
	{
		m_materials[index] = material;
		save();
	}

	bool Model::hasSubmeshMaterial(size_t index) const
	{
		return m_materials.find(index) != m_materials.end();
	}

	std::shared_ptr<Material> Model::getSubmeshMaterial(size_t index) const
	{
		if (hasSubmeshMaterial(index))
		{
			return m_materials.at(index);
		}
		return nullptr;
	}

	std::unordered_map<size_t, std::shared_ptr<Material>> Model::getMaterials() const
	{
		return m_materials;
	}

	ComponentInfo Model::getInfo()
	{
		ComponentInfo displayInfo;

		displayInfo.setComponentName("Model");
		std::shared_ptr<FieldFile> modelFile = std::make_shared<FieldFile>();
		modelFile->name = "PathToModelFile";
		if (!m_path.empty())
		{
			modelFile->path = m_path;
		}
		modelFile->callback = [this](const std::string& path)
		{
			std::shared_ptr<Model> newModel = AssetManager::LoadModel(path);
			newModel->setParent(this->getParent());
			*this = *newModel;

		};
		displayInfo.addElement(modelFile);

		return displayInfo;
	}
	void Model::save()
	{
		nlohmann::json data;
		data["PathToMesh"] = m_mesh->getPath();
		std::vector<std::shared_ptr<SubMesh>> submeshes = m_mesh->getSubmeshes();
		for (size_t i = 0; i < submeshes.size(); i++)
		{
			if (hasSubmeshMaterial(i))
			{
				data["PathToSubmeshMaterial" + std::to_string(i)] = getSubmeshMaterial(i)->getPath();
			}
		}
		std::string pathToModelFile;
		if (getPath().empty())
		{
			if (m_mesh->getPath().string().find("Primitive::") != std::string::npos)
			{
				size_t i = 0;
				while (std::filesystem::exists("Primitive" + std::to_string(i) + ".model"))
				{
					i++;
				}
				pathToModelFile = "Primitive" + std::to_string(i) + ".model";
			}
			else
			{
				pathToModelFile = m_mesh->getPath().parent_path().string() + "/" + m_mesh->getPath().stem().string() + ".model";
			}
		}
		else
		{
			pathToModelFile = getPath().parent_path().string() + "/" + getPath().stem().string() + ".model";
		}
		setPath(pathToModelFile);
		std::ofstream file(pathToModelFile, std::ios_base::out);
		if (file.is_open())
		{
			file << data.dump(4);
			file.close();
		}
		else
		{
			Logger::Critical("ERROR::Model::Failed to save the model file!");
		}
	}
}
