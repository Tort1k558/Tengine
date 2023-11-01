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
		modelFile->name = "Model";
		if (!m_path.empty())
		{
			modelFile->path = m_path;
		}
		modelFile->callback = [this](const std::string& path)
		{
			std::shared_ptr<Object> previousParent = this->getParent();
			if (path == "Primitive::Quad")
			{
				m_mesh = Primitives::CreateQuad();

			}
			else if (path == "Primitive::Cube")
			{
				m_mesh = Primitives::CreateCube();
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
				m_mesh = Primitives::CreateSphere(sectors, stacks);
			}
			else
			{
				m_mesh = AssetManager::LoadMesh(path);
			}
			this->setParent(previousParent);
		};
		displayInfo.addElement(modelFile);
		//std::shared_ptr<FieldCollapsingHeader> submeshesHeader = std::make_shared<FieldCollapsingHeader>();
		//submeshesHeader->name = "Submeshes";
		//std::vector<std::shared_ptr<SubMesh>> submeshes = m_mesh->getSubmeshes();
		//for (size_t i = 0; i < submeshes.size(); i++)
		//{
		//	std::shared_ptr<FieldCollapsingHeader> submeshHeader = std::make_shared<FieldCollapsingHeader>();
		//	submeshHeader->name = "Submesh" + std::to_string(i);
		//	std::shared_ptr<Material> material = getSubmeshMaterial(i);
		//	if (material)
		//	{
		//		std::shared_ptr<FieldFile> materialFile = std::make_shared<FieldFile>();
		//		materialFile->name = "Material";
		//		materialFile->path = material->getPath();
		//		materialFile->callback = [this, i](const std::string& path)
		//		{
		//			this->setSubmeshMaterial(i,AssetManager::LoadMaterial(path));
		//		};
		//		//std::shared_ptr<FieldImage> albedo = std::make_shared<FieldImage>();
		//		//albedo->name = "Albedo Texture";
		//		//albedo->size = { 75,75 };
		//		//std::shared_ptr<FieldImage> specular = std::make_shared<FieldImage>();
		//		//specular->name = "Specular Texture";
		//		//specular->size = { 75,75 };
		//		//std::shared_ptr<FieldImage> normals = std::make_shared<FieldImage>();
		//		//normals->name = "Normal Texture";
		//		//normals->size = { 75,75 };
		//		//std::shared_ptr<FieldImage> height = std::make_shared<FieldImage>();
		//		//height->name = "Height Texture";
		//		//height->size = { 75,75 };
		//		//std::shared_ptr<FieldImage> roughness = std::make_shared<FieldImage>();
		//		//roughness->name = "Roughness Texture";
		//		//roughness->size = { 75,75 };
		//		//std::shared_ptr<FieldImage> metalness = std::make_shared<FieldImage>();
		//		//metalness->name = "Metalness Texture";
		//		//metalness->size = { 75,75 };
		//		//if (material)
		//		//{
		//		//	albedo->texture = material->getTexture(MaterialTexture::Diffuse);
		//		//	specular->texture = material->getTexture(MaterialTexture::Specular);
		//		//	normals->texture = material->getTexture(MaterialTexture::Normal);
		//		//	height->texture = material->getTexture(MaterialTexture::Height);
		//		//	roughness->texture = material->getTexture(MaterialTexture::Roughness);
		//		//	metalness->texture = material->getTexture(MaterialTexture::Metalness);
		//		//}
		//		//submeshHeader->elements.push_back(albedo);
		//		//submeshHeader->elements.push_back(specular);
		//		//submeshHeader->elements.push_back(normals);
		//		//submeshHeader->elements.push_back(height);
		//		//submeshHeader->elements.push_back(roughness);
		//		//submeshHeader->elements.push_back(metalness);
		//		submeshHeader->elements.push_back(materialFile);
		//	}
		//	submeshesHeader->elements.push_back(submeshHeader);
		//}
		//displayInfo.addElement(submeshesHeader);

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
			pathToModelFile = m_mesh->getPath().parent_path().string() + "/" + m_mesh->getPath().stem().string() + ".model";
		}
		else if (m_mesh->getPath().string().find("Primitive::") != std::string::npos)
		{
			size_t i = 0;
			while (std::filesystem::exists("Primitive" + std::to_string(i) + ".model"))
			{
				i++;
			}
			pathToModelFile = std::filesystem::exists("Primitive" + std::to_string(i) + ".model");
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
