#include "Model.h"

#include<fstream>

#include"Core/Logger.h"
#include"Utils/Primitives.h"

namespace Tengine
{
	std::shared_ptr<Mesh> Model::getMesh() const
	{
		return m_mesh;
	}

	void Model::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
		AssetManager::SaveModel(this);
	}

	void Model::setSubmeshMaterial(size_t index, std::shared_ptr<Material> material)
	{
		m_materials[index] = material;
		AssetManager::SaveModel(this);
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
}
